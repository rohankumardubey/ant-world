#include "../include/world.h"

#include "../include/player.h"
#include "../include/food.h"
#include "../include/hole.h"
#include "../include/npc_ant.h"
#include "../include/cookie.h"
#include "../include/mm.h"
#include "../include/food_spawner.h"
#include "../include/container.h"
#include "../include/vehicle.h"
#include "../include/beetle.h"

aw::World::World() {
    m_mainRealm = nullptr;
    m_player = nullptr;
}

aw::World::~World() {
    /* void */
}

void aw::World::initialize(void *instance, ysContextObject::DEVICE_API api) {
    dbasic::Path modulePath = dbasic::GetModulePath();
    dbasic::Path confPath = modulePath.Append("delta.conf");

    std::string enginePath = "../../dependencies/delta/engines/basic";
    std::string assetPath = "../../assets";
    if (confPath.Exists()) {
        std::fstream confFile(confPath.ToString(), std::ios::in);

        std::getline(confFile, enginePath);
        std::getline(confFile, assetPath);
        enginePath = modulePath.Append(enginePath).ToString();
        assetPath = modulePath.Append(assetPath).ToString();

        confFile.close();
    }

    m_assetPath = dbasic::Path(assetPath);

    m_engine.GetConsole()->SetDefaultFontDirectory(enginePath + "/fonts/");

    m_engine.CreateGameWindow(
        "Ant World",
        instance,
        api,
        (enginePath + "/shaders/").c_str());

    m_assetManager.SetEngine(&m_engine);

    AssetLoader::loadAllAssets(dbasic::Path(assetPath), &m_assetManager);

    m_worldGrid.initialize(10.0f);
    m_worldGrid.setWorld(this);
}

void aw::World::initialSpawn() {
    m_mainRealm = newRealm<Realm>();
    m_mainRealm->setIndoor(false);

    //Food *leaf1 = m_mainRealm->spawn<Food>();
    //leaf1->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f));

    //Food *leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 2.0f, 0.0f, 0.0f));

    FoodSpawner *spawner = m_mainRealm->spawn<FoodSpawner>();
    spawner->RigidBody.Transform.SetPosition(ysMath::Constants::Zero3);
    spawner->setAveragePeriod(2.0f);
    spawner->setLifespan(3600.0f);
    spawner->setType(FoodSpawner::Type::Cookie);
    spawner->setRadius(10.0f);

    Beetle *beetle = m_mainRealm->spawn<Beetle>();
    beetle->RigidBody.Transform.SetPosition(ysMath::LoadVector(10.0f, 0.0f, 0.0));

    //Hole *hole = m_mainRealm->spawn<Hole>();
    //hole->RigidBody.SetPosition(ysMath::LoadVector(0.0f, 5.0f, 0.0f, 1.0f));

    //Container *container = m_mainRealm->spawn<Container>();
    //container->RigidBody.SetPosition(ysMath::LoadVector(5.0f, 0.0f, 0.0f, 1.0f));

    //Vehicle *vehicle = m_mainRealm->spawn<Vehicle>();
    //vehicle->RigidBody.Transform.SetPosition(ysMath::LoadVector(-5.0f, 0.0f, 0.0f, 1.0f));
    //m_player = vehicle;
    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(-1.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(-2.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(-3.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(1.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(2.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(3.0f, -5.0f, 0.0f, 0.0f));

    //leaf2 = m_mainRealm->spawn<Food>();
    //leaf2->RigidBody.SetPosition(ysMath::LoadVector(4.0f, -5.0f, 0.0f, 0.0f));

    //NpcAnt *npc = m_mainRealm->spawn<NpcAnt>();
    //npc->RigidBody.SetPosition(ysMath::LoadVector(0.0f, -10.0f, 0.0f, 0.0f));

    //npc = m_mainRealm->spawn<NpcAnt>();
    //npc->RigidBody.SetPosition(ysMath::LoadVector(-10.0f, 0.0f, 0.0f, 0.0f));

    //npc = m_mainRealm->spawn<NpcAnt>();
    //npc->RigidBody.SetPosition(ysMath::LoadVector(10.0f, 0.0f, 0.0f, 0.0f));

    m_player = m_mainRealm->spawn<Player>();

    int px, py;
    px = ysMath::GetX(m_player->RigidBody.Transform.GetWorldPosition()) / 10.0f;
    py = ysMath::GetY(m_player->RigidBody.Transform.GetWorldPosition()) / 10.0f;
    for (int i = -50; i < 50; ++i) {
        for (int j = -50; j < 50; ++j) {
            m_worldGrid.requestFragment({ px + i, py + j });
        }
    }
}

void aw::World::run() {
    initialSpawn();

    while (m_engine.IsOpen()) {
        frameTick();
    }
}

void aw::World::frameTick() {
    m_engine.StartFrame();

    updateRealms();

    process();
    render();

    m_engine.EndFrame();
}

/*
int color[] = { 0xf1, 0xc4, 0x0f };
ysMatrix translation = ysMath::TranslationTransform(ysMath::LoadVector(-3.0f, 0.0f, 0.0f));
m_engine.SetObjectTransform(translation);
m_engine.DrawBox(color, 2.5f, 2.5f);

m_engine.SetMultiplyColor(ysVector4(1.0f, 1.0f, 1.0f, 1.0f));
m_engine.SetObjectTransform(ysMath::LoadIdentity());
m_engine.DrawImage(m_demoTexture, 0, (float)m_demoTexture->GetWidth() / m_demoTexture->GetHeight());
*/

aw::AABB aw::World::getCameraExtents() const {
    float cameraX, cameraY;
    m_engine.GetCameraPosition(&cameraX, &cameraY);

    float altitude = m_engine.GetCameraAltitude();

    float v, h;
    float fov = m_engine.GetCameraFov();
    float aspect = m_engine.GetCameraAspect();

    v = std::tan(fov / 2) * altitude;
    h = v * aspect;

    return { ysMath::LoadVector(-h + cameraX, -v + cameraY, 0.0f, 0.0f), ysMath::LoadVector(h + cameraX, v + cameraY) };
}

void aw::World::render() {
    ysVector playerPosition = m_player->RigidBody.Transform.GetWorldPosition();

    m_engine.SetCameraPosition(ysMath::GetX(playerPosition), ysMath::GetY(playerPosition));

    if (m_engine.IsKeyDown(ysKeyboard::KEY_V)) {
        m_engine.SetCameraAltitude(500.0f);
    }
    else m_engine.SetCameraAltitude(30.0f); // 30.0f

    m_player->getRealm()->render();

    if (m_player->getRealm() == m_mainRealm) {
        m_worldGrid.debugRender();
    }
}

void aw::World::process() {
    int px, py;
    px = ysMath::GetX(m_player->RigidBody.Transform.GetWorldPosition()) / 10.0f;
    py = ysMath::GetY(m_player->RigidBody.Transform.GetWorldPosition()) / 10.0f;
    for (int i = -5; i < 5; ++i) {
        for (int j = -5; j < 5; ++j) {
            m_worldGrid.requestFragment({ px + i, py + j });
        }
    }

    m_player->getRealm()->process();
}

void aw::World::updateRealms() {
    for (Realm *realm : m_realms) {
        realm->updateRealms();
    }
}

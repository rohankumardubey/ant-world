#include "../include/mm.h"

#include "../include/world.h"

dbasic::TextureAsset *aw::MM::MMTexture = nullptr;

aw::MM::MM() {
    /* void */
}

aw::MM::~MM() {
    /* void */
}

void aw::MM::initialize() {
    GameObject::initialize();

    setNutritionalValue(1.0f);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(10.0f);
    //RigidBody.SetInverseInertiaTensor(ysMath::LoadIdentity());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsCircle()->RadiusSquared = 0.45f * 0.45f;

    dphysics::CollisionObject *sensor;
    RigidBody.CollisionGeometry.NewCircleObject(&sensor);
    sensor->SetMode(dphysics::CollisionObject::Mode::Sensor);
    sensor->GetAsCircle()->RadiusSquared = 2.0f * 2.0f;
}

void aw::MM::render() {
    m_world->getEngine().SetObjectTransform(RigidBody.GetTransform());
    m_world->getEngine().SetMultiplyColor(ysVector4(1.0f, 1.0f, 1.0f, 1.0f));
    m_world->getEngine().DrawImage(MMTexture->GetTexture(), (int)Layer::Items, 0.5f, 0.5f);
}

void aw::MM::process() {
    /* void */
}

void aw::MM::configureAssets(dbasic::AssetManager *am) {
    MMTexture = am->GetTexture("MM");
}

#include "../include/world_grid.h"

#include "../include/world_fragment.h"

#include <assert.h>

aw::WorldGrid::WorldGrid() {
    m_fragmentSize = 10.0f;
    m_world = nullptr;
}

aw::WorldGrid::~WorldGrid() {
    /* void */
}

void aw::WorldGrid::initialize(double fragmentSize) {
    m_fragmentSize = fragmentSize;

    m_biomes.push_back(new SavannaBiome);
    m_biomes.push_back(new DesertBiome);
    m_biomes.push_back(new ContinentalWarmBiome);
    m_biomes.push_back(new ContinentalColdBiome);
    m_biomes.push_back(new ArcticBiome);
    m_biomes.push_back(new TropicalBiome);

    int seed = 2001;

    m_precipitation.setScale(1.0 / 100);
    m_precipitation.setSeed(seed);

    m_temperatureFluctuation.setScale(1.0 / 200);
    m_temperatureFluctuation.setSeed(seed + 1);

    m_averageTemperature.setScale(1.0 / 200);
    m_averageTemperature.setSeed(seed + 2);

    m_elevation.setScale(1.0 / 200.0);
    m_elevation.setSeed(seed + 3);
}

void aw::WorldGrid::debugRender() {
    for (auto fragment : m_fragments) {
        fragment.second->debugRender();
    }
}

void aw::WorldGrid::setWorld(World *world) {
    m_world = world;
}

aw::WorldFragment *aw::WorldGrid::newFragment(const FragmentCoord &coord) {
    aw::WorldFragment *newFragment = new aw::WorldFragment;
    
    double posx = coord.x * m_fragmentSize;
    double posy = coord.y * m_fragmentSize;

    Biome::BiomeParameters param;
    param.Precipitation = m_precipitation.perlin(posx, posy);
    param.AverageTemperature = m_averageTemperature.perlin(posx, posy);
    param.TemperatureFluctation = m_temperatureFluctuation.perlin(posx, posy);
    param.Elevation = m_elevation.perlin(posx, posy);

    Biome::Type type = getBiomeType(param);
    assert(type != Biome::Type::Unknown);

    newFragment->initialize(coord.x, coord.y, m_fragmentSize, m_fragmentSize, type, param);
    newFragment->setWorld(m_world);

    return newFragment;
}

aw::Biome::Type aw::WorldGrid::getBiomeType(
    const Biome::BiomeParameters &parameters) const 
{
    float minDeviation = 0;
    Biome *closestBiome = nullptr;

    for (Biome *biome : m_biomes) {
        float score = biome->checkBiome(parameters);
        if (closestBiome == nullptr || score < minDeviation) {
            closestBiome = biome;
            minDeviation = score;
        }
    }

    if (closestBiome == nullptr) return Biome::Type::Unknown;
    else return closestBiome->getType();
}

aw::WorldFragment *aw::WorldGrid::requestFragment(const FragmentCoord &coord) {
    unsigned __int64 hash = PerlinNoiseGenerator::szudzikHash(coord.x, coord.y);

    auto f = m_fragments.find(hash);
    if (f == m_fragments.end()) {
        return m_fragments[hash] = newFragment(coord);
    }
    else return f->second;
}

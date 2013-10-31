#ifndef AVALON_IO_TILEDMAPLOADER_H
#define AVALON_IO_TILEDMAPLOADER_H

#include "cocos2d.h"
#include <avalon/physics/Box2dContainer.h>

namespace avalon {
namespace io {

class TiledMapLoader
{
public:
    typedef std::map<std::string, std::string> Dictionary;
    struct Configuration
    {
        const Dictionary& settings;
        const std::string& layer;
        cocos2d::TMXTiledMap& map;
        avalon::physics::Box2dContainer* box2dContainer;
    };
    typedef std::function<void(const Configuration&)> Callback;

private:
    std::map<int, Callback> gidFactories;
    std::map<std::string, Callback> nameFactories;

    avalon::physics::Box2dContainer* box2dContainer = nullptr;
    const std::string mapFileName;

    bool isFiltered(const std::string& name, const std::list<std::string> filter = {});
    void loadGidFactories(cocos2d::TMXTiledMap& map);
    void loadNamedFactories(cocos2d::TMXTiledMap& map);
    
public:
    TiledMapLoader(const std::string mapFileName);
    std::shared_ptr<cocos2d::TMXTiledMap> load();
    void registerCallbackForName(const std::string& name, const Callback& callback, const std::list<std::string> layerFilter = {});
    void setBox2dContainer(avalon::physics::Box2dContainer& container);

    template<typename T>
    void registerObjectForGID(const int gid, const std::list<std::string> layerFilter = {})
    {
        gidFactories[gid] = [this, layerFilter](const Configuration& config)
        {
            if (!isFiltered(config.layer, layerFilter)) {
                auto newObject = T::create();
                newObject->onConfiguration(config);
                config.map.addChild(newObject);
            }
        };
    }

    template<typename T>
    void registerObjectForName(const std::string& name, const std::list<std::string> layerFilter = {})
    {
        nameFactories[name] = [this, layerFilter](const Configuration& config)
        {
            if (!isFiltered(config.layer, layerFilter)) {
                auto newObject = T::create();
                newObject->onConfiguration(config);
                config.map.addChild(newObject);
            }
        };
    }
};


} // namespace io
} // namespace avalon

#endif /* AVALON_IO_TILEDMAPLOADER_H */
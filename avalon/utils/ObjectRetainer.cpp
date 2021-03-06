#include <avalon/utils/ObjectRetainer.h>

namespace avalon {
namespace utils {

ObjectRetainer::~ObjectRetainer()
{
    clear();
}

void ObjectRetainer::add(cocos2d::Ref& obj)
{
    obj.retain();
    objects.push_back(&obj);
}

void ObjectRetainer::clear()
{
    std::for_each(
        objects.begin(), objects.end(),
        [](cocos2d::Ref* obj) { obj->release(); });
    objects.clear();
}

} // namespace utils
} // namespace avalon

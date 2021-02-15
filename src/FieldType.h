#ifndef FieldType_hpp
#define FieldType_hpp
enum class FieldType { Wall, Point, Free };
#define fieldTypeToString(ft) (ft == FieldType::Wall ? "Wall" : (ft == FieldType::Point ? "Point" : ("Free")))
#endif /* FieldType_hpp */
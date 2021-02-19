#ifndef FieldType_hpp
#define FieldType_hpp
enum class FieldType { Wall, Point, Free, Portal };
#define fieldTypeToString(ft) (ft == FieldType::Wall ? "Wall" : (ft == FieldType::Point ? "Point" : (ft == FieldType::Free ? "Free" : "Portal")))
#endif /* FieldType_hpp */
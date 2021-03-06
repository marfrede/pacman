#ifndef FieldType_hpp
#define FieldType_hpp
enum class FieldType { Wall, Point, Free, Portal, OutOfField };
#define fieldTypeToString(ft) (ft == FieldType::Wall ? "Wall" : (ft == FieldType::Point ? "Point" : (ft == FieldType::Free ? "Free" : (ft == FieldType::Portal ? "Portal" : "OutOfField"))))
#endif /* FieldType_hpp */
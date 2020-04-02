#ifndef CG1_PROJECT_COMPONENT_H
#define CG1_PROJECT_COMPONENT_H

enum ComponentType {
    Mesh,
    Position,
    Velocity
};

template <class T>
class Component {
public:
    bool init(T& data);

private:
    T data;
};

template<class T>
bool Component<T>::init(T& data) {
    this->data = data;
    return true;
}


#endif //CG1_PROJECT_COMPONENT_H

#ifndef CG1_PROJECT_NOISE_H
#define CG1_PROJECT_NOISE_H


class Noise {
public:
    virtual ~Noise() {}

    virtual double evaluate(float x, float y) = 0;
    virtual double evaluate(float x, float y, float z) = 0;
};


#endif //CG1_PROJECT_NOISE_H

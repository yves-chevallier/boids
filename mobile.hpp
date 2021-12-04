#include "vector.hpp"

class Mobile {
protected:
    Vector position;
    Vector velocity;

    bool wrap;
    float max_velocity;

public:
    float angle();
    float speed();

    float angleTo(Mobile &other);
    float distanceTo(Mobile &other);

    /**
     * Edges
     */
    void bounce(float margin, float turnFactor);
    void wrap();

    void update();    
};
#include "vector.hpp"
#include <deque>

class Mobile {
protected:
    bool wrap;
    float max_velocity;

    std::deque<Vector> history;
    int max_history;

public:
    Vector position;
    Vector velocity;

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
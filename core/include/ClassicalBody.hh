#include <Body.hh>

class ClassicalBody : public Body {
    public:
        ClassicalBody() {return;};
        ~ClassicalBody() {return;};

        void update(double dt) override;
};

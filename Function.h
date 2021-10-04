namespace Functions {
    class Function {
    public:
        virtual double operator()(double x, double y, double t) = 0;
    };

    class Plane : Function {
        double operator()(double x, double y, double t) override;
    };

    class Spikes : Function {
        double operator()(double x, double y, double t) override;
    };
}

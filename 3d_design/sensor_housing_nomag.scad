$fs=0.1; $fa=0.1;
union(){
    translate(v=[0, 0, -7.6]){
        cube(size=[47.7, 29.2, 1], center=true);
    };
    translate(v=[0, 0, 0.6999999999999993]){
        difference(){
            cube(size=[47.7, 29.2, 15.599999999999998], center=true);
            cube(size=[45.7, 27.2, 15.609999999999998], center=true);
        };
    };
    translate(v=[13.97, 10.16, -4.35]){
        difference(){
            cylinder(h=5.5, d=5, center=true);
            cylinder(h=6, d=2, center=true);
        };
    };
    translate(v=[13.97, -10.16, -4.35]){
        difference(){
            cylinder(h=5.5, d=5, center=true);
            cylinder(h=6, d=2, center=true);
        };
    };
    translate(v=[-8.97, 10.16, -4.35]){
        cylinder(h=5.5, d=3, center=true);
    };
    translate(v=[-8.97, -10.16, -4.35]){
        cylinder(h=5.5, d=3, center=true);
    };
    translate(v=[-20.35, -11.1, 0.6999999999999993]){
        difference(){
            cube(size=[5, 5, 15.599999999999998], center=true);
            translate(v=[0, 0, 2.799999999999999]){
                cylinder(h=10, d=2);
            };
        };
    };
    translate(v=[-20.35, 11.1, 0.6999999999999993]){
        difference(){
            cube(size=[5, 5, 15.599999999999998], center=true);
            translate(v=[0, 0, 2.799999999999999]){
                cylinder(h=10, d=2);
            };
        };
    };
    translate(v=[20.35, -11.1, 0.6999999999999993]){
        difference(){
            cube(size=[5, 5, 15.599999999999998], center=true);
            translate(v=[0, 0, 2.799999999999999]){
                cylinder(h=10, d=2);
            };
        };
    };
    translate(v=[20.35, 11.1, 0.6999999999999993]){
        difference(){
            cube(size=[5, 5, 15.599999999999998], center=true);
            translate(v=[0, 0, 2.799999999999999]){
                cylinder(h=10, d=2);
            };
        };
    };
};

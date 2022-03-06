$fs=0.1; $fa=0.1;
union(){
    difference(){
        union(){
            translate(v=[0, 0, -8.95]){
                cube(size=[46.7, 29.2, 3.7], center=true);
            };
            translate(v=[0, 0, 0.6999999999999993]){
                difference(){
                    cube(size=[46.7, 29.2, 15.599999999999998], center=true);
                    cube(size=[44.7, 27.2, 15.609999999999998], center=true);
                };
            };
        };
        translate(v=[-10.0, 0, -8.399999999999999]){
            cylinder(h=2.8000000000000003, d=11, center=true);
        };
        translate(v=[10.0, 0, -8.399999999999999]){
            cylinder(h=2.8000000000000003, d=11, center=true);
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
    translate(v=[-19.85, -11.1, 0.6999999999999993]){
        difference(){
            cube(size=[5, 5, 15.599999999999998], center=true);
            translate(v=[0, 0, 2.799999999999999]){
                cylinder(h=10, d=2);
            };
        };
    };
    translate(v=[-19.85, 11.1, 0.6999999999999993]){
        difference(){
            cube(size=[5, 5, 15.599999999999998], center=true);
            translate(v=[0, 0, 2.799999999999999]){
                cylinder(h=10, d=2);
            };
        };
    };
    translate(v=[19.85, -11.1, 0.6999999999999993]){
        difference(){
            cube(size=[5, 5, 15.599999999999998], center=true);
            translate(v=[0, 0, 2.799999999999999]){
                cylinder(h=10, d=2);
            };
        };
    };
    translate(v=[19.85, 11.1, 0.6999999999999993]){
        difference(){
            cube(size=[5, 5, 15.599999999999998], center=true);
            translate(v=[0, 0, 2.799999999999999]){
                cylinder(h=10, d=2);
            };
        };
    };
};

$fs=0.1; $fa=0.1;
union(){
    difference(){
        hull(){
            union(){
                translate(v=[-2.0, -2.0, -7]){
                    cylinder(h=34.5, d=2);
                };
                translate(v=[-2.0, 60.6, -7]){
                    cylinder(h=34.5, d=2);
                };
                translate(v=[80.8, -2.0, -7]){
                    cylinder(h=34.5, d=2);
                };
                translate(v=[80.8, 60.6, -7]){
                    cylinder(h=34.5, d=2);
                };
            };
        };
        translate(v=[-1, -1, -5.5]){
            cube(size=[80.8, 60.6, 34.0]);
        };
    };
    translate(v=[2.5, 2.5, -5.5]){
        difference(){
            translate(v=[-3.5, -3.5, 0]){
                cube(size=[7, 7, 5.5]);
            };
            translate(v=[0, 0, -0.25]){
                cylinder(h=6, d=4);
            };
        };
    };
    translate(v=[2.5, 56.1, -5.5]){
        difference(){
            translate(v=[-3.5, -3.5, 0]){
                cube(size=[7, 7, 5.5]);
            };
            translate(v=[0, 0, -0.25]){
                cylinder(h=6, d=4);
            };
        };
    };
    translate(v=[76.3, 2.5, -5.5]){
        difference(){
            translate(v=[-3.5, -3.5, 0]){
                cube(size=[7, 7, 5.5]);
            };
            translate(v=[0, 0, -0.25]){
                cylinder(h=6, d=4);
            };
        };
    };
    translate(v=[76.3, 56.1, -5.5]){
        difference(){
            translate(v=[-3.5, -3.5, 0]){
                cube(size=[7, 7, 5.5]);
            };
            translate(v=[0, 0, -0.25]){
                cylinder(h=6, d=4);
            };
        };
    };
};

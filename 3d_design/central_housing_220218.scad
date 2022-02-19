$fs=0.1; $fa=0.1;
difference(){
    union(){
        difference(){
            hull(){
                union(){
                    translate(v=[-2.0, -2.0, -7]){
                        cylinder(h=33.5, d=2);
                    };
                    translate(v=[-2.0, 60.6, -7]){
                        cylinder(h=33.5, d=2);
                    };
                    translate(v=[80.8, -2.0, -7]){
                        cylinder(h=33.5, d=2);
                    };
                    translate(v=[80.8, 60.6, -7]){
                        cylinder(h=33.5, d=2);
                    };
                };
            };
            translate(v=[-1, -1, -5.5]){
                cube(size=[80.8, 60.6, 33.0]);
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
    translate(v=[-3.5, 5.8, -1.1]){
        cube(size=[3, 11, 8]);
    };
    translate(v=[-3.5, 42.5, -1.1]){
        cube(size=[3, 11, 8]);
    };
    translate(v=[79.3, 10, 20]){
        rotate(a=[0, 90, 0]){
            cylinder(h=3, d=6.5);
        };
    };
    translate(v=[79.3, 45, 20]){
        rotate(a=[0, 90, 0]){
            cylinder(h=3, d=3.6);
        };
    };
};

$fs=0.1; $fa=0.1;
color([0.5, 0.5, 0.5]){
    difference(){
        union(){
            difference(){
                union(){
                    difference(){
                        union(){
                            difference(){
                                union(){
                                    difference(){
                                        hull(){
                                            union(){
                                                translate(v=[-2.0, -2.0, 26.5]){
                                                    cylinder(h=2.0, d=2);
                                                };
                                                translate(v=[-2.0, 60.6, 26.5]){
                                                    cylinder(h=2.0, d=2);
                                                };
                                                translate(v=[80.8, -2.0, 26.5]){
                                                    cylinder(h=2.0, d=2);
                                                };
                                                translate(v=[80.8, 60.6, 26.5]){
                                                    cylinder(h=2.0, d=2);
                                                };
                                            };
                                        };
                                        translate(v=[-1, -1, 0]){
                                            cube(size=[80.8, 60.6, 0]);
                                        };
                                    };
                                    translate(v=[-0.5, -0.5, 21.5]){
                                        cube(size=[6, 6, 5]);
                                    };
                                };
                                translate(v=[2.5, 2.5, 21.4]){
                                    cylinder(h=7.2, d=3.2);
                                };
                            };
                            translate(v=[-0.5, 53.1, 21.5]){
                                cube(size=[6, 6, 5]);
                            };
                        };
                        translate(v=[2.5, 56.1, 21.4]){
                            cylinder(h=7.2, d=3.2);
                        };
                    };
                    translate(v=[73.3, -0.5, 21.5]){
                        cube(size=[6, 6, 5]);
                    };
                };
                translate(v=[76.3, 2.5, 21.4]){
                    cylinder(h=7.2, d=3.2);
                };
            };
            translate(v=[73.3, 53.1, 21.5]){
                cube(size=[6, 6, 5]);
            };
        };
        translate(v=[76.3, 56.1, 21.4]){
            cylinder(h=7.2, d=3.2);
        };
    };
};

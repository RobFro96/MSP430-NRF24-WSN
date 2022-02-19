$fs=0.1; $fa=0.1;
union(){
    difference(){
        color([1, 0, 0]){
            cube(size=[78.8, 58.6, 1.6]);
        };
        translate(v=[2.5, 2.5, -1]){
            cylinder(h=3.6, d=3);
        };
        translate(v=[2.5, 56.1, -1]){
            cylinder(h=3.6, d=3);
        };
        translate(v=[76.3, 2.5, -1]){
            cylinder(h=3.6, d=3);
        };
        translate(v=[76.3, 56.1, -1]){
            cylinder(h=3.6, d=3);
        };
    };
    color([0.5, 0.5, 0.5]){
        translate(v=[0, 7.5, 1.6]){
            cube(size=[5.1, 7.6, 2.6]);
        };
    };
    color([0.5, 0.5, 0.5]){
        translate(v=[0, 44.5, 1.6]){
            cube(size=[3.7, 7, 3.5]);
        };
    };
    color([0.1, 0.1, 0.1]){
        translate(v=[40, 5, 1.6]){
            cube(size=[26, 2.5, 9.7]);
        };
    };
    color([0.1, 0.1, 0.1]){
        translate(v=[40, 51.1, 1.6]){
            cube(size=[26, 2.5, 9.7]);
        };
    };
    translate(v=[27.799999999999997, 4.300000000000001, 11.299999999999999]){
        union(){
            color([0, 1, 0]){
                cube(size=[51, 50, 1.5]);
            };
            color([0.1, 0.1, 0.1]){
                translate(v=[6.5, 14, 1.5]){
                    cube(size=[5.1, 10.7, 9.7]);
                };
            };
            color([0.1, 0.1, 0.1]){
                translate(v=[44.8, 11.2, 1.5]){
                    cube(size=[2.5, 2.5, 9.7]);
                };
            };
            color([0.1, 0.1, 0.1]){
                translate(v=[44.8, 24, 1.5]){
                    cube(size=[2.5, 2.5, 9.7]);
                };
            };
            translate(v=[6.2, 11.7, 11.2]){
                union(){
                    color([0.2, 0.2, 0.2]){
                        cube(size=[40.9, 15.3, 1]);
                    };
                    color([1, 1, 0]){
                        translate(v=[33.2, 4.9, 1]){
                            cube(size=[6, 6, 2.5]);
                        };
                    };
                };
            };
        };
    };
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
                cylinder(h=3, d=5.2);
            };
        };
    };
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
};

$fs=0.1; $fa=0.1;
difference(){
    union(){
        difference(){
            union(){
                difference(){
                    union(){
                        difference(){
                            union(){
                                translate(v=[0, 0, -1]){
                                    cube(size=[59, 47, 2], center=true);
                                };
                                translate(v=[-26.67, -17.78, 0]){
                                    cylinder(h=4, d=5.5);
                                };
                            };
                            translate(v=[-26.67, -17.78, -1.5]){
                                cylinder(h=5.6, d=1.5);
                            };
                        };
                        translate(v=[26.67, -17.78, 0]){
                            cylinder(h=4, d=5.5);
                        };
                    };
                    translate(v=[26.67, -17.78, -1.5]){
                        cylinder(h=5.6, d=1.5);
                    };
                };
                translate(v=[26.67, 17.78, 0]){
                    cylinder(h=4, d=5.5);
                };
            };
            translate(v=[26.67, 17.78, -1.5]){
                cylinder(h=5.6, d=1.5);
            };
        };
        translate(v=[-26.67, 20.32, 0]){
            cylinder(h=4, d=5.5);
        };
    };
    translate(v=[-26.67, 20.32, -1.5]){
        cylinder(h=5.6, d=1.5);
    };
};

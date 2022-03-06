$fs=0.1; $fa=0.1;
color([1, 0, 0]){
    translate(v=[0, 0, 8.999999999999998]){
        difference(){
            cube(size=[46.7, 29.2, 1], center=true);
            translate(v=[-19.85, -11.1]){
                cylinder(h=1.1, d=2.5, center=true);
            };
            translate(v=[-19.85, 11.1]){
                cylinder(h=1.1, d=2.5, center=true);
            };
            translate(v=[19.85, -11.1]){
                cylinder(h=1.1, d=2.5, center=true);
            };
            translate(v=[19.85, 11.1]){
                cylinder(h=1.1, d=2.5, center=true);
            };
        };
    };
};

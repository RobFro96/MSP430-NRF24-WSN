from openpyscad import Cube, Cylinder, Empty

RED = [1, 0, 0]
YELLOW = [1, 1, 0]
GREEN = [0, 1, 0]
CYAN = [0, 1, 1]
BLUE = [0, 0, 1]
MAGENTA = [1, 0, 1]
DGRAY = [.2]*3
GRAY = [.5]*3
BLACK = [.1]*3
WHITE = [1]*3

holes_dx = 21*2.54
holes_dy = 14*2.54
holes_x = [-holes_dx / 2, holes_dx / 2, holes_dx / 2, -holes_dx / 2]
holes_y = [-holes_dy/2, -holes_dy/2, holes_dy/2, holes_dy/2+2.54]
standoff_z = 4


def save_obj(obj, filename, header="$fs=0.1; $fa=0.1;"):
    open(filename, "w", encoding="utf8").write(header + "\n" + obj.dumps())


out = Cube([59, 47, 2], center=True).translate([0, 0, -1])
for x, y in zip(holes_x, holes_y):
    out += Cylinder(d=5.5, h=standoff_z).translate([x, y, 0])
    out -= Cylinder(d=1.5, h=standoff_z+1.5 + 0.1).translate([x, y, -1.5])

pcb = Cube([59, 45, 1.6], center=True)
for x, y in zip(holes_x, holes_y):
    pcb -= Cylinder(d=2.5, h=2, center=True).translate([x, y, 0])

pcb = pcb.translate([0, 0, 0.8 + standoff_z]).color(GREEN)
# out += pcb

save_obj(out, __file__[:-2] + "scad")

import itertools

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


def create_housing(inner_x, inner_y, d, fill_z, remove_z):
    obj = Empty()
    poles_x = [inner_x[0] - d/2, inner_x[1] + d/2]
    poles_y = [inner_y[0] - d/2, inner_y[1] + d/2]

    for x, y in itertools.product(poles_x, poles_y):
        obj += Cylinder(d=d, h=fill_z[1] - fill_z[0]).translate([x, y, fill_z[0]])

    obj = obj.hull()
    obj -= Cube([inner_x[1] - inner_x[0], inner_y[1]-inner_y[0], remove_z[1] - remove_z[0]]) \
        .translate([inner_x[0], inner_y[0], remove_z[0]])
    return obj


def save_obj(obj, filename, header="$fs=0.1; $fa=0.1;"):
    open(filename, "w", encoding="utf8").write(header + "\n" + obj.dumps())


pcb_xs = 33.7
pcb_ys = 26.2
pcb_zs = 1.6

pcb = Cube([pcb_xs, pcb_ys, pcb_zs], center=True).color(GREEN)
pcb -= Cylinder(h=pcb_zs+1, d=2.2, center=True).translate([13.97, 10.16, 0])
pcb -= Cylinder(h=pcb_zs+1, d=2.2, center=True).translate([13.97, -10.16, 0])
pcb = pcb.translate([0, 0, -pcb_zs/2])

nrf_xs = 28.6
nrf_ys = 15.43
nrf_zs = 1.2

nrf = Cube([nrf_xs, nrf_ys, nrf_zs], center=True).color(BLACK)
crystal = Cube([10, 4.5, 3.5], center=True).color(WHITE).translate([-3, -4, nrf_zs/2 + 3.5/2])
nrf += crystal

nrf = nrf.translate([nrf_xs/2-pcb_xs/2, pcb_ys/2 - nrf_ys/2, 3.3 + nrf_zs/2])
pcb += nrf

battery = Cube([22, 16, 5.5], center=True).color(GRAY)
battery = battery.translate([0, -1.27, -5.5/2-pcb_zs])
pcb += battery

housing_xs = pcb_xs + 13
housing_ys = 26.2 + 3
housing_zs = pcb_zs+5.5+3.3+1.2+3.5 + .5
d = 1
magnet_d = 10
magnet_h = 2.7


housing = Cube([housing_xs, housing_ys, d + magnet_h],
               center=True).translate([0, 0, -pcb_zs-5.5-d/2 - magnet_h/2])

walls = Cube([housing_xs, housing_ys, housing_zs], center=True)
walls -= Cube([housing_xs-2*d, housing_ys-2*d, housing_zs+.01], center=True)
walls = walls.translate([0, 0, housing_zs/2 - pcb_zs-5.5])
housing += walls

for x in [-magnet_d/2-5, magnet_d/2+5]:
    magnet_hole = Cylinder(d=magnet_d+1, h=magnet_h+.1,
                           center=True).translate([x, 0, -pcb_zs-5.5-magnet_h/2+.05])
    housing -= magnet_hole

standoff_x = [13.97, 13.97, 5-13.97, 5-13.97]
standoff_y = [10.16, -10.16, 10.16, -10.16]
for x, y, stand_d, hole in zip(standoff_x, standoff_y, [5, 5, 3, 3], [True, True, False, False]):
    standoff = Cylinder(d=stand_d, h=5.5, center=True)
    if hole:
        standoff -= Cylinder(d=2, h=6, center=True)

    housing += standoff.translate([x, y, -5.5/2-pcb_zs])

top_hole_x = [-housing_xs/2+3.5, housing_xs/2-3.5]
top_hole_y = [-housing_ys/2+3.5, housing_ys/2-3.5]
for x, y in itertools.product(top_hole_x, top_hole_y):
    standoff = Cube([5, 5, housing_zs], center=True)
    standoff -= Cylinder(d=2, h=10).translate([0, 0, housing_zs/2-5])

    housing += standoff.translate([x, y, housing_zs/2 - pcb_zs-5.5])

top = Cube([housing_xs, housing_ys, d], center=True)
for x, y in itertools.product(top_hole_x, top_hole_y):
    top -= Cylinder(d=2.5, h=d+.1, center=True).translate([x, y])

top = top.translate([0, 0, housing_zs - pcb_zs-5.5 + d/2]).color(RED)

# obj = pcb + housing + top
# obj -= Cube([100, 100, 100]).translate([-50, 0, -50])
# obj = housing
obj = top
save_obj(obj, __file__[:-2] + "scad")

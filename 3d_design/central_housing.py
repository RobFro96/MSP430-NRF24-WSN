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

lp_xs = 78.8
lp_ys = 58.6
lp_zs = 1.6
hole_xpos = [2.5, lp_xs-2.5]
hole_ypos = [2.5, lp_ys-2.5]
shield_xs = 51
shield_ys = 50
shield_zs = 1.5
nrf_xs = 40.9
nrf_ys = 15.3
nrf_zs = 1

housing_height = lp_zs + 9.7 + shield_zs + 9.7 + nrf_zs + 3


def save_obj(obj, filename, header="$fs=0.1; $fa=0.1;"):
    open(filename, "w", encoding="utf8").write(header + "\n" + obj.dumps())


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


def create_launchpad():
    lp = Cube([lp_xs, lp_ys, lp_zs])
    lp = lp.color(RED)

    for x, y in itertools.product(hole_xpos, hole_ypos):
        lp -= Cylinder(h=lp_zs+2, d=3).translate([x, y, -1])

    usb = Cube([5.1, 7.6, 2.6]).translate([0, 7.5, lp_zs]).color(GRAY)
    lp += usb

    rst_btn = Cube([3.7, 7, 3.5]).translate([0, lp_ys-7-7.1, lp_zs]).color(GRAY)
    lp += rst_btn

    for y in [5, lp_ys - 5 - 2.5]:
        header = Cube([26, 2.5, 9.7]).translate([40, y, lp_zs]).color(BLACK)
        lp += header

    shield = Cube([shield_xs, shield_ys, shield_zs]).color(GREEN)

    header1 = Cube([5.1, 10.7, 9.7]).translate([6.5, 14, shield_zs]).color(BLACK)
    shield += header1
    for y in [11.2, 24]:
        header = Cube([2.5, 2.5, 9.7]).translate([shield_xs-3.7-2.5, y, shield_zs]).color(BLACK)
        shield += header

    nrf = Cube([nrf_xs, nrf_ys, nrf_zs]).color(DGRAY)

    rf_jack = Cube([6, 6, 2.5]).translate([33.2, 4.9, nrf_zs]).color(YELLOW)
    nrf += rf_jack

    shield += nrf.translate([6.2, 11.7, 9.7 + shield_zs])
    lp += shield.translate([lp_xs - shield_xs, (lp_ys-shield_ys)/2, 9.7+lp_zs])
    return lp


# HOUSING
housing = create_housing([-1, lp_xs+1], [-1, lp_ys+1], 2,
                         [-7, housing_height], [-5.5, housing_height + 1])

for hole_x, hole_y in itertools.product(hole_xpos, hole_ypos):
    insert = Cube([7, 7, 5.5]).translate([-3.5, -3.5, 0]) - \
        Cylinder(d=4, h=6).translate([0, 0, -.25])
    insert = insert.translate([hole_x, hole_y, -5.5])
    housing += insert

usb_hole = Cube([3, 11, 8]).translate([-3.5, 7.5 - (11-7.6)/2, lp_zs - (8-2.6)/2])
housing -= usb_hole

rst_btn_hole = Cube([3, 11, 8]).translate([-3.5, lp_ys-7-7.1 - (11-7)/2, lp_zs - (8-2.6)/2])
housing -= rst_btn_hole

antenna_hole = Cylinder(d=6.5, h=3).rotate([0, 90, 0]).translate([lp_xs + .5, 10, 20])
housing -= antenna_hole

led_hole = Cylinder(d=5.2, h=3).rotate([0, 90, 0]).translate([lp_xs + .5, 45, 20])
housing -= led_hole

# LID
lid = create_housing([-1, lp_xs+1], [-1, lp_ys+1], 2,
                     [housing_height, housing_height+2], [0, 0])
for hole_x, hole_y in itertools.product(hole_xpos, hole_ypos):
    aligner = Cube([6, 6, 5]).translate([hole_x-3, hole_y-3, housing_height-5])
    hole = Cylinder(d=3.2, h=7.2).translate([hole_x, hole_y, housing_height-5.1])
    lid = lid + aligner - hole

lid = lid.color(GRAY)

cut = Cube([100, 8, 100]).translate([0, -5, 0])

out = create_launchpad() + housing + lid
save_obj(out, __file__[:-2] + "scad")

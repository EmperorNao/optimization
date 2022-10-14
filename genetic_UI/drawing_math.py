from math import floor
from math import sin, cos
from math import pi


class Point:

    def __init__(self, x=None,y=None):
        self.x = x
        self.y = y


def get_radians(angle_in_degrees):
    return angle_in_degrees * pi / 180


def get_points(center, radius, number_of_points):

    points = []

    angle = get_radians(360 / number_of_points)

    for point_idx in range(number_of_points):
        x = center.x + radius * sin(angle * point_idx)
        y = center.y + radius * cos(angle * point_idx)
        points.append(Point(int(x), int(y)))

    return points
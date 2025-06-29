##
## EPITECH PROJECT, 2025
## header
## File description:
## ai zappy class
##

from dataclasses import dataclass
from enum import Enum

class Direction(Enum):
    NORTH = 0
    EAST = 1
    SOUTH = 2
    WEST = 3

@dataclass
class Resources:
    food: int = 0
    linemate: int = 0
    deraumere: int = 0
    sibur: int = 0
    mendiane: int = 0
    phiras: int = 0
    thystame: int = 0

@dataclass
class ElevationRequirement:
    nb_players: int
    linemate: int
    deraumere: int
    sibur: int
    mendiane: int
    phiras: int
    thystame: int

ELEVATION_REQUIREMENTS = {
    1: ElevationRequirement(1, 1, 0, 0, 0, 0, 0),
    2: ElevationRequirement(2, 1, 1, 1, 0, 0, 0),
    3: ElevationRequirement(2, 2, 0, 1, 0, 2, 0),
    4: ElevationRequirement(4, 1, 1, 2, 0, 1, 0),
    5: ElevationRequirement(4, 1, 2, 1, 3, 0, 0),
    6: ElevationRequirement(6, 1, 2, 3, 0, 1, 0),
    7: ElevationRequirement(6, 2, 2, 2, 2, 2, 1),
}

RESOURCE_NAMES = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
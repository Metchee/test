##
## EPITECH PROJECT, 2025
## header
## File description:
## protocol
##

from typing import List 
from game_types import Resources 

class ProtocolParser:
    def parse_inventory(self, response: str) -> Resources:
        inventory = Resources() 
        if not response or not response.startswith("[") or not response.endswith("]"):
            print(f"ProtocolParser: Invalid inventory format: '{response}'")
            return inventory 

        content = response[1:-1].strip() 
        if not content: 
            return inventory

        items = content.split(',')
        for item_entry in items:
            parts = item_entry.strip().split()
            if len(parts) == 2:
                name = parts[0].lower() 
                try:
                    quantity = int(parts[1])
                    if hasattr(inventory, name):
                        setattr(inventory, name, quantity)
                    else:
                        print(f"ProtocolParser: Unknown resource '{name}' in inventory string.")
                except ValueError:
                    print(f"ProtocolParser: Invalid quantity for resource '{name}' in inventory string: '{parts[1]}'")
            elif item_entry.strip(): 
                print(f"ProtocolParser: Malformed item entry in inventory: '{item_entry.strip()}'")
        return inventory

    def parse_look_result(self, response: str) -> List[List[str]]:
        parsed_tiles: List[List[str]] = []
        if not response or not response.startswith("[") or not response.endswith("]"):
            print(f"ProtocolParser: Invalid look format: '{response}'")
            return [] 

        content = response[1:-1] 
        if not content.strip() and content != "": 
            parsed_tiles.append([]) 
            return parsed_tiles
        tiles_str_list = content.split(',')
        for tile_content_str in tiles_str_list:
            objects_on_tile = tile_content_str.strip().split() 
            parsed_tiles.append(objects_on_tile)
            
        return parsed_tiles

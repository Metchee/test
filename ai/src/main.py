##
## EPITECH PROJECT, 2025
## header
## File description:
## ai zappy py
##

import sys
import argparse
from zappy_ai import ZappyAI

def parse_arguments():
    parser = argparse.ArgumentParser(description='Zappy AI Client')
    parser.add_argument('-p', '--port', type=int, required=True, help='Port number')
    parser.add_argument('-n', '--name', type=str, required=True, help='Team name')
    parser.add_argument('-m', '--machine', type=str, default='localhost', help='Server hostname')
    return parser.parse_args()

def main():
    try:
        args = parse_arguments()
        ai = ZappyAI(args.machine, args.port, args.name)
        ai.run()
    except KeyboardInterrupt:
        print("\nAI stopping...")
        sys.exit(0)
    except Exception as e:
        print(f"Error in main: {e}")
        sys.exit(84)

if __name__ == "__main__":
    main()
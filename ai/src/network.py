##
## EPITECH PROJECT, 2025
## header
## File description:
## ai network zappy
##

import socket
import sys
from typing import Optional, Tuple

class NetworkClient:
    def __init__(self, host: str, port: int, team_name: str):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.socket: Optional[socket.socket] = None
        self.is_connected = False
        self.pending_commands = 0
        self.max_pending = 10
        self._read_buffer = b""

    def connect(self) -> bool:
        if self.is_connected:
            return True
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(5)
            self.socket.connect((self.host, self.port))
            self.is_connected = True
            self.pending_commands = 0
            self._read_buffer = b""
            print(f"Network: Connected to {self.host}:{self.port}")
            return True
        except Exception as e:
            print(f"Network: Connection failed - {e}")
            self.socket = None
            return False

    def disconnect(self):
        if self.socket:
            try:
                self.socket.close()
            except:
                pass
            self.socket = None
        self.is_connected = False

    def send_command(self, command: str) -> bool:
        if not self.is_connected or not self.socket:
            return False
        
        if self.pending_commands >= self.max_pending:
            print(f"Network: Too many pending commands, skipping: {command}")
            return False

        try:
            message = f"{command}\n"
            self.socket.sendall(message.encode('utf-8'))
            self.pending_commands += 1
            return True
        except Exception as e:
            print(f"Network: Send failed: {e}")
            self.is_connected = False
            return False

    def receive_line(self) -> str:
        if not self.is_connected or not self.socket:
            return "dead"
        
        newline_idx = self._read_buffer.find(b"\n")

        while newline_idx == -1:
            try:
                data = self.socket.recv(1024)
                if not data:
                    self.is_connected = False
                    return "dead"
                self._read_buffer += data
                newline_idx = self._read_buffer.find(b"\n")
            except socket.timeout:
                return ""
            except Exception as e:
                print(f"Network: Receive failed: {e}")
                self.is_connected = False
                return "dead"

        message_line_bytes = self._read_buffer[:newline_idx]
        self._read_buffer = self._read_buffer[newline_idx+1:]

        message_str = message_line_bytes.decode('utf-8', errors='ignore').strip()

        if message_str.startswith("message"):
            return message_str
        elif message_str.startswith("eject:"):
            return message_str
        elif message_str == "dead":
            return message_str
        else:
            if message_str and message_str not in ["", "ko", "ok"]:
                self.pending_commands = max(0, self.pending_commands - 1)
            elif message_str in ["ok", "ko"]:
                self.pending_commands = max(0, self.pending_commands - 1)

        return message_str

    def authenticate(self) -> Tuple[bool, Tuple[int, int]]:
        if not self.is_connected:
            return False, (0, 0)
        try:
            welcome = self.receive_line()
            if welcome != "WELCOME":
                return False, (0, 0)
            
            if not self.send_command(self.team_name):
                return False, (0, 0)
            
            client_num = self.receive_line()
            if client_num == "ko":
                return False, (0, 0)
            
            dimensions = self.receive_line()
            if dimensions:
                dim_parts = dimensions.strip().split()
                if len(dim_parts) >= 2:
                    world_size = (int(dim_parts[0]), int(dim_parts[1]))
                    return True, world_size
            return False, (0, 0)
        except Exception as e:
            print(f"Network: Authentication failed: {e}")
            return False, (0, 0)
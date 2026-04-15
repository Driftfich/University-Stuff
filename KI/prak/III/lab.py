from graph_search import GraphSearch
from typing import Self
from PIL import Image, ImageDraw

class Node:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def getNeighbors(self, up: bool, down: bool, left: bool, right: bool) -> list[Self]:
        neighbors = []
        if up:
            neighbors.append(Node(self.x, self.y - 1))
        if down:
            neighbors.append(Node(self.x, self.y + 1))
        if left:
            neighbors.append(Node(self.x - 1, self.y))
        if right:
            neighbors.append(Node(self.x + 1, self.y))
        return neighbors

    def __hash__(self):
        return hash((self.x, self.y))

    def __repr__(self) -> str:
        return f"Node(x={self.x}, y={self.y})"

class Labyrinth(GraphSearch):
    adj_coors: dict[Node, list[Node]] = {
        Node(0,0): Node(0,0).getNeighbors(False, False, False, True),
        Node(1,0): Node(1,0).getNeighbors(False, True, True, True),
        Node(2, 0): [Node(1,0), Node(3, 0)],
        Node(3, 0): [Node(2, 0)],
        Node(4, 0): [Node(4,1)],

        Node(0,1): [Node(0,2)],
        Node(1,1): [Node(1,0), Node(1,2), Node(2,1)],
        Node(2,1): [Node(1,1)],
        Node(3,1): [Node(3,2), Node(4,1)],
        Node(4,1): [Node(3,1), Node(4,0)],

        Node(0,2): [Node(0,1), Node(1,2), Node(0,3)],
        Node(1,2): [Node(1,1), Node(0,2)],
        Node(2,2): [Node(2,3), Node(3,2)],
        Node(3,2): Node(3,2).getNeighbors(True, True, True, False),
        Node(4,2): Node(4,2).getNeighbors(False, True, False, False),

        Node(0,3): Node(0,3).getNeighbors(True, True, False, True),
        Node(1,3): Node(1,3).getNeighbors(False, True, True, False),
        Node(2,3): Node(2,3).getNeighbors(True, True, False, False),
        Node(3,3): Node(3,3).getNeighbors(True, False, False, True),
        Node(4,3): Node(4,3).getNeighbors(True, True, True, False),

        Node(0,4): Node(0,4).getNeighbors(True, False, False, False),
        Node(1,4): Node(1,4).getNeighbors(True, False, False, True),
        Node(2,4): Node(2,4).getNeighbors(True, False, True, True),
        Node(3,4): Node(3,4).getNeighbors(False, False, True, True),
        Node(4,4): Node(4,4).getNeighbors(True, False, True, False),
    }

    def __init__(self, node: Node):
        self.node = node

    @classmethod
    def show_labyrinth_image(cls, path_result: list['Labyrinth'] | None = None, filename: str = "labyrinth_structure.png"):
        """Generiert ein Bild, das die Graphenstruktur des Labyrinths zeigt und optional einen Pfad hervorhebt."""

        print(f"Generiere Bild: {filename}...")

        CELL_SIZE = 80 # Größe eines Quadrats im Grid
        OFFSET = 40    # Rand um das Grid
        RADIUS = 10    # Radius der Knoten
        LINE_WIDTH = 3 # Breite der Graphen-Linien
        PATH_LINE_WIDTH = 10 # Breite der Pfad-Hervorhebung
        GRID_SIZE = 5

        img_size = (GRID_SIZE * CELL_SIZE + 2 * OFFSET, GRID_SIZE * CELL_SIZE + 2 * OFFSET)
        img = Image.new("RGB", img_size, color="white")
        draw = ImageDraw.Draw(img)

        # Hilfsfunktion, um Graphen-Koordinaten (0,0) - (4,4) in Pixel umzurechnen
        def to_pixel(node: Node):
            # (0,0) ist oben links, x wächst nach rechts, y wächst nach unten
            return (node.x * CELL_SIZE + OFFSET + CELL_SIZE // 2,
                    node.y * CELL_SIZE + OFFSET + CELL_SIZE // 2)

        # 1. Kanten (Verbindungen) des gesamten Graphen zeichnen (Grau)
        seen_edges = set() # Um jede Kante nur einmal zu zeichnen
        for node, neighbours in cls.adj_coors.items():
            u_px = to_pixel(node)
            for neighbour in neighbours:
                edge = tuple(sorted((node, neighbour), key=lambda n: (n.x, n.y))) # Ungerichtete Kante
                if edge not in seen_edges:
                    v_px = to_pixel(neighbour)
                    draw.line([u_px, v_px], fill="lightgray", width=LINE_WIDTH)
                    seen_edges.add(edge)

        # 2. Einen gefundenen Pfad hervorheben (Blau)
        if path_result:
            path_nodes = [l_instance.node for l_instance in path_result]
            for i in range(len(path_nodes) - 1):
                u_node, v_node = path_nodes[i], path_nodes[i+1]
                u_px = to_pixel(u_node)
                v_px = to_pixel(v_node)
                draw.line([u_px, v_px], fill="royalblue", width=PATH_LINE_WIDTH)

        # 3. Knoten-Punkte zeichnen und markieren
        unique_nodes = set(cls.adj_coors.keys())
        for neighbours in cls.adj_coors.values():
            unique_nodes.update(neighbours)

        for node in unique_nodes:
            px = to_pixel(node)
            color = "white"
            outline_color = "black"

            # Start/Ziel/Pfad-spezifische Farben
            if path_result and node in path_nodes:
                color = "lightblue"
            if node == Node(0,0):
                color = "green"
            elif node == Node(4,0):
                color = "red"

            # Kreis zeichnen
            draw.ellipse([px[0] - RADIUS, px[1] - RADIUS, px[0] + RADIUS, px[1] + RADIUS],
                            fill=color, outline=outline_color, width=2)

            # Text-Label (optional, um Koordinaten zu sehen)
            # draw.text((px[0] + 15, px[1] - 10), f"({node.x},{node.y})", fill="black")

        img.save(filename)
        print("Bild erfolgreich gespeichert.")

    def adj(self) -> list[Self]:
        return [Labyrinth(node) for node in self.adj_coors[self.node]]

    def goal(self) -> bool:
        return self.node == Node(4,0)

    def __hash__(self) -> int:
        return hash(self.node)

# pat
# Labyrinth.show_labyrinth_image()
#

if __name__ == "__main__":
    lab = Labyrinth(Node(0,0))
    path = lab.bfs([lab])
    node_path = [node.node for node in path]
    if path:
        print(f"Pfad gefunden: {node_path}")
        Labyrinth.show_labyrinth_image(path)

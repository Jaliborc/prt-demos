package formats.pgv;
import org.lwjgl.util.vector.Vector2f;

public class Edge {
	public Vector2f coords;
	public Vertex vertex;
	
	public Edge(Vertex vertex) {
		this.vertex = vertex;
	}

	public Edge(Vertex vertex, Vector2f coords) {
		this.vertex = vertex;
		this.coords = coords;
	}
}

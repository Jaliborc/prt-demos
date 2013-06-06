package formats.pgv;
import org.lwjgl.util.vector.Matrix4f;
import org.lwjgl.util.vector.Vector2f;

import math.Vector;

public class Vertex {
	public Vertex(int index) {
		this.index = index;
	}
	
	public Vector position = new Vector();
	public Vector normal = new Vector();
	public Matrix4f visibility = new Matrix4f();
	public int index;
}

package formats.pgv;
import org.lwjgl.util.vector.Matrix4f;
import org.lwjgl.util.vector.Vector2f;

import math.Vector;

public class Vertex {
	public Vector position = new Vector();
	public Vector normal = new Vector();
	public Vector2f coords = new Vector2f();
	public Matrix4f visibility = new Matrix4f();
}

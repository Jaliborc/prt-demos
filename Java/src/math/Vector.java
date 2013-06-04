package math;
import org.lwjgl.util.vector.Vector3f;

public class Vector extends Vector3f {
	public Vector() {}
	public Vector(float x, float y, float z) {
		super(x, y, z);
	}
	public Vector(Vector other) {
		super(other);
	}

	public void add(Vector other) {
		super.add(this, other, this);
	}
	
	public void sub(Vector other) {
		super.sub(this, other, this);
	}
	
	public void cross(Vector other) {
		super.cross(this, other, this);
	}
	
	public String toString() {
		return x + " " + y + " " + z;
	}
}

package formats.obj;
import java.util.ArrayList;
import math.Vector;

public class Face extends ArrayList<Vertex> {
	public Vector normal() {
		Vector a = new Vector(get(1).position);
		a.sub(get(0).position);
		
		Vector b = new Vector(get(2).position);
		b.sub(get(0).position);
		
		a.cross(b);
		return a;
	}
}
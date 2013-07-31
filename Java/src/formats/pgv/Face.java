package formats.pgv;
import java.util.ArrayList;
import math.Vector;

public class Face extends ArrayList<Edge> {
	public Vector normal() {
		Vector a = new Vector(get(1).vertex.position);
		a.sub(get(0).vertex.position);
		
		Vector b = new Vector(get(2).vertex.position);
		b.sub(get(0).vertex.position);
		
		a.cross(b);
		return a;
	}
}

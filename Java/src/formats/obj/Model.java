package formats.obj;
import static java.lang.Float.parseFloat;
import static java.lang.Integer.parseInt;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import math.Vector;

public class Model {
	public ArrayList<Face> faces = new ArrayList<Face>();
	public ArrayList<Vertex> vertices = new ArrayList<Vertex>();
	public ArrayList<Vector> normals = new ArrayList<Vector>();
	
	public Model(String path) throws Exception {
		BufferedReader file = new BufferedReader(new InputStreamReader(new FileInputStream(path)));
		String line;
		
		while( (line = file.readLine()) != null) {
			String[] parts = line.split("\\s+");
			String type = parts[0];
			
			if (type.equals("f"))
				faces.add(ParseFace(parts));
			else if (type.equals("v"))
				vertices.add(ParseVertex(parts));
			else if (type.equals("vn"))
				normals.add(ParseVector(parts));
		}
		
		for (Face face : faces) {
			Vector n = face.normal();
			
			for (Vertex vertex : face)
				vertex.normal.add(n);
		}
		
		for (Vertex vertex : vertices)
			if (vertex.normal.lengthSquared() > 0)
				vertex.normal.normalise();
		
		file.close();
	}
	
	Face ParseFace(String[] parts) {
		Face face = new Face();
		
		for (int i = 1; i< parts.length; i++) {
			String[] values = parts[i].split("/");
			Vertex vertex = vertices.get(parseInt(values[0]) - 1);
			
			face.add(vertex);
		}
		
		return face;
	}
	
	Vertex ParseVertex(String[] values) {
		Vertex vertex = new Vertex();
		vertex.position = ParseVector(values);
		vertex.normal = new Vector();
		return vertex;
	}
	
	Vector ParseVector(String[] values) {
		return new Vector(
			parseFloat(values[1]),
			parseFloat(values[2]),
			parseFloat(values[3])
		);
	}
}

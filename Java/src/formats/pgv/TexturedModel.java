package formats.pgv;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.util.ArrayList;

import org.lwjgl.util.vector.Vector2f;

import math.Svd;

public class TexturedModel extends Model {
	public ArrayList<Vector2f> coords = new ArrayList<Vector2f>();
	
	public TexturedModel(String file) throws Exception {
		ObjectInputStream stream = new ObjectInputStream(new FileInputStream(file));
		
		positions = new Svd(stream);
		visibility = new Svd(stream);
		
		for (int i = 0; i < positions.height() / 3; i++)
			vertices.add(new Vertex(vertices.size()));
		
		int[] c = (int[]) stream.readObject();
		for (int i = 0; i < c.length;)
			coords.add(new Vector2f(c[i++], c[i++]));
		
		int[] f = (int[]) stream.readObject();
		for (int i = 0; i < f.length;) {
			Face face = new Face();
			
			for (int k = 0; k < 3; k++)
				face.add(new Edge(vertices.get(f[i++]), coords.get(f[i++])));
			
			faces.add(face);
		}
		
		stream.close();
	}
}

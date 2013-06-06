package formats.pgv;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.util.ArrayList;

import math.Vector;
import math.Svd;

public class Model {
	public ArrayList<Face> faces = new ArrayList<Face>();
	
	public Model(String file) throws Exception {
		ObjectInputStream stream = new ObjectInputStream(new FileInputStream(file));
		
		positions = new Svd(stream);
		visibility = new Svd(stream);
		
		for (int i = 0; i < positions.height() / 3; i++)
			vertices.add(new Vertex(vertices.size()));
		
		int[] f = (int[]) stream.readObject();
		for (int i = 0; i < f.length;) {
			Face face = new Face();
			face.add(vertices.get(f[i++]));
			face.add(vertices.get(f[i++]));
			face.add(vertices.get(f[i++]));
			
			faces.add(face);
		}
		
		stream.close();
	}
	
	public void setPose(int pose) {
		float[] pos = positions.get(pose);
		float[] vis = visibility.get(pose);
		int i = 0, j = 0;
		
		for (Vertex vertex : vertices) {
			vertex.normal.set(0, 0, 0);
			vertex.position.set(pos[i++], pos[i++], pos[i++]);
			
			vertex.visibility.m00 = vis[j++];
			vertex.visibility.m01 = vis[j++];
			vertex.visibility.m02 = vis[j++];
			vertex.visibility.m03 = vis[j++];
			vertex.visibility.m10 = vis[j++];
			vertex.visibility.m11 = vis[j++];
			vertex.visibility.m12 = vis[j++];
			vertex.visibility.m13 = vis[j++];
			vertex.visibility.m20 = vis[j++];
			vertex.visibility.m21 = vis[j++];
			vertex.visibility.m22 = vis[j++];
			vertex.visibility.m23 = vis[j++];
			vertex.visibility.m30 = vis[j++];
			vertex.visibility.m31 = vis[j++];
			vertex.visibility.m32 = vis[j++];
			vertex.visibility.m33 = vis[j++];
		}
		
		for (Face face : faces) {
			Vector n = face.normal();
			
			for (Vertex vertex : face)
				vertex.normal.add(n);
		}
		
	}
	
	public int numPoses() {
		return positions.width();
	}
	
	Svd positions, visibility;
	ArrayList<Vertex> vertices = new ArrayList<Vertex>();
}

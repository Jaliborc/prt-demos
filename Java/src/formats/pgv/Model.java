package formats.pgv;
import java.util.ArrayList;
import math.Svd;
import math.Vector;

public abstract class Model {
	public ArrayList<Face> faces = new ArrayList<Face>();
	
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
			
			for (Edge edge : face)
				edge.vertex.normal.add(n);
		}
		
	}
	
	public int numPoses() {
		return positions.width();
	}
	
	Svd positions, visibility;
	ArrayList<Vertex> vertices = new ArrayList<Vertex>();
}

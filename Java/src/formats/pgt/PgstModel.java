package formats.pgt;
import java.io.DataInputStream;
import java.io.FileInputStream;

import static math.Matrix.readInt;
import math.IntMatrix;
import math.Svd;

public class PgstModel extends Model {
	public Svd geometry;
	public Svd[] bands;
	
	public PgstModel(String file) throws Exception {
		DataInputStream stream = new DataInputStream(new FileInputStream(file));
		
		geometry = new Svd(stream);
		vertices = new Vertex[geometry.height() / 3];
		faces = new IntMatrix(stream).values;
		bands = new Svd[readInt(stream)];
		
		for (int i = 0; i < bands.length; i++)
			bands[i] = new Svd(stream);
		
		for (int i = 0; i < vertices.length; i++)
			vertices[i] = new Vertex();
		
		stream.close();
		
		System.out.println(numBands());
		System.out.println(numHarmonics());
	}
	
	public void render(int pose, int harmonic, float[] radiance) {
		float[] positions = geometry.get(pose);
		int i = 0;
		
		for (Vertex vertex : vertices) {
			vertex.set(positions[i++], positions[i++], positions[i++]);
			vertex.r = 0; vertex.g = 0; vertex.b = 0;
		}
		
		int start = 0, size = 3;
		for (Svd band : bands) {
			float[] transfer = band.get(pose);
			int end = start + size;
			int j = 0;
			
			for (Vertex vertex : vertices)
				for (int n = start; n < end;) {
					vertex.r += transfer[j++] * radiance[n++];
					vertex.g += transfer[j++] * radiance[n++];
					vertex.b += transfer[j++] * radiance[n++];
				}
			
			start = end;
			size = size + 6;
		}
	}

	public int numPoses() {
		return geometry.width();
	}

	public int numBands() {
		return bands.length;
	}

	public int numHarmonics() {
		return (bands.length * bands.length) * 3;
	}
}

package formats.pgt;
import static java.lang.Math.pow;

import java.io.DataInputStream;
import java.io.FileInputStream;

import math.IntMatrix;
import math.Svd;

public class PgtModel extends Model {
	public Svd geometry, transfer;
	
	public PgtModel(String file) throws Exception {
		DataInputStream stream = new DataInputStream(new FileInputStream(file));
		
		geometry = new Svd(stream);
		vertices = new Vertex[geometry.height() / 3];
		faces = new IntMatrix(stream).values;
		transfer = new Svd(stream);
		
		stream.close();
	}
	
	public void render(int pose, int harmonic, float[] radiance) {
		float[] positions = geometry.get(pose);
		float[] colors = transfer.get(pose);
		int i = 0, j = harmonic * 3;
		
		for (Vertex vertex : vertices) {
			vertex.set(positions[i++], positions[i++], positions[i++]);
			vertex.r = 0; vertex.g = 0; vertex.b = 0;
			
			if (harmonic == 0)
				for (int n = 0; n < numHarmonics();) {
					vertex.r += colors[j++] * radiance[n++];
					vertex.g += colors[j++] * radiance[n++];
					vertex.b += colors[j++] * radiance[n++];
				}
			else {
				float color = colors[j];
				vertex.r = -color;
				vertex.b = color;
				
				j += numHarmonics();
			}
		}
	}
	
	public int numPoses() {
		return geometry.width();
	}
	
	public int numBands() {
		return (int) pow(numHarmonics(), .5);
	}
	
	public int numHarmonics() {
		return transfer.height() / vertices.length;
	}
}

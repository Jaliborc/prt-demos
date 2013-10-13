package formats.pgt;

public abstract class Model {
	public Vertex[] vertices;
	public int[] faces;
	
	public abstract void render(int pose, int harmonic, int numBands, float[] radiance);
	public abstract int numPoses();
	public abstract int numBands();
	public abstract int numHarmonics();
}

package exporters;
import static java.lang.String.format;
import java.io.BufferedWriter;
import java.io.FileWriter;
import formats.obj.*;

public class Pbrt {
	public static void main(String[] args) throws Exception {
		Export("../Models/horse", "transfer", 3);
	}
	
	public static void Export(String path, String mode, int harmonicLevel) throws Exception {
		for (String file : IO.getFiles(path, ".obj"))
			new Pbrt(file, mode, harmonicLevel);
	}
	
	public Pbrt(String path, String mode, int harmonicLevel) throws Exception {
		String name = IO.removeExtension(path);
		Model model = new Model(path);
		
		output = new BufferedWriter(new FileWriter(name + ".pbrt"));
		output.write(format(Renderer, name + "." + mode, mode, harmonicLevel));
		
		Write(model.vertices);
		
		output.write("WorldBegin\n");
		
		for (Face face: model.faces)
			Write(face);
		
		output.write("WorldEnd");
		output.close();
	}
	
	void Write(Face face) throws Exception {
		output.write("Shape \"trianglemesh\"  \"integer indices\" [0 1 2] ");
		Write((Iterable<Vertex>) face);
	}
	
	void Write(Iterable<Vertex> vertices) throws Exception {
		String points = "";
		String normals = "";
		
		int section = 0;
		int i = 0;
		
		for (Vertex vertex : vertices) {
			points += vertex.position.toString() + " ";
			normals += vertex.normal.toString() + " ";
			i++;
			
			if (i % 200 == 0) {
				output.write(ToPBRT(points, normals, section));
				normals = "";
				points = "";
				section++;
			}
		}
		
		output.write(ToPBRT(points, normals, section) + "\n");
	}
	
	String ToPBRT(String points, String normals, int id) {
		return String.format(Vertices, id, points, id, normals);
	}
	
	BufferedWriter output;
	String Renderer = "Renderer \"precomputeTransfer\" \"string filename\" \"%s\" \"string mode\" \"%s\" \"integer harmonicLevel\" %s";
	String Vertices = "\"point P%s\" [%s] \"normal N%s\" [%s] ";
}

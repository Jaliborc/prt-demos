package exporters;
import static java.lang.Integer.parseInt;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import formats.obj.*;

public class Merger {
	public static void main(String[] args) throws Exception {
		Merge("model/Octopus & Dense Floor", "model/Floor/Dense.obj");
	}
	
	public static void Merge(String targets, String annex) throws Exception {
		String[] files = IO.getFiles(targets, ".obj");
		
		for (String file : files)
			Append(file, annex);
	}

	public static void Append(String target, String annex) throws Exception {
		Model model = new Model(target);
		int vertexOff = model.vertices.size();
		int normalOff = model.normals.size();
		
		PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(target, true)));
		BufferedReader floor = new BufferedReader(new InputStreamReader(new FileInputStream(annex)));
		String line;
		
		out.println("");
		
		while( (line = floor.readLine()) != null) {
			if (line.startsWith("v"))
				out.println(line);
			else if (line.startsWith("f")) {
				Matcher matches = pattern.matcher(line);
				out.print("f");
				
				for (int i = 0; i < 3; i++) {
					matches.find();
					int vertex = parseInt(matches.group());
					matches.find();
					int normal = parseInt(matches.group());
					
					out.print(" " + (vertex + vertexOff) + "//" + (normal + normalOff));
				}
				
				out.println("");
			}
		}	
		
		out.close();
	}
	
	static Pattern pattern = Pattern.compile("\\d+");
}

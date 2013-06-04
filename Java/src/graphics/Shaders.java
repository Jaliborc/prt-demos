package graphics;
import static org.lwjgl.opengl.GL11.GL_TRUE;
import static org.lwjgl.opengl.GL20.*;

import java.io.BufferedReader;
import java.io.FileReader;

public class Shaders {
	public static int link(int ... shaders) throws Exception {
		int id = glCreateProgram();
		
		for (int shader : shaders)
			glAttachShader(id, shader);

		glLinkProgram(id);
		return id;
	}
	
	public static int compile(String file, int type) throws Exception {
		BufferedReader reader = new BufferedReader(new FileReader(file));
		StringBuilder source = new StringBuilder();
		
		char[] chars = new char[2048];
		int count;
		
		while ((count = reader.read(chars)) > 0)
			source.append(chars, 0, count);
		
		int id = glCreateShader(type);
		glShaderSource(id, source);
		glCompileShader(id);
		
		if (glGetShaderi(id, GL_COMPILE_STATUS) != GL_TRUE)
			throw new Exception(file + " failed to compile:\n" + glGetShaderInfoLog(id, 2000));
		
		return id;
	}
}

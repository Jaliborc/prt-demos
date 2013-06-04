package viewer;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL13.*;
import static org.lwjgl.opengl.GL20.*;

import org.newdawn.slick.opengl.Texture;
import org.newdawn.slick.opengl.TextureLoader;
import org.newdawn.slick.util.ResourceLoader;

import java.nio.FloatBuffer;

import formats.pgv.Face;
import formats.pgv.Vertex;
import formats.pgv.Model;

import graphics.Buffers;
import graphics.Presenter;
import graphics.Shaders;

public class Pgv extends Presenter {
	public Pgv(String file) throws Exception {
		model = new Model(file);
		scene = new Scene("../Ambients/Street.tga");
		shaders = Shaders.link(
			Shaders.compile("shader/SHVisibility.vert", GL_VERTEX_SHADER),
			Shaders.compile("shader/SHVisibility.frag", GL_FRAGMENT_SHADER));
		
		model.setPose(0);
	    
		int light = glGetUniformLocation(shaders, "light");
	    FloatBuffer floats = Buffers.allocateFloats(48);
	    
		for (int i = 0; i < 48; i+=3)
			floats.put(scene.radiance[i]);
		
		for (int i = 1; i < 48; i+=3)
			floats.put(scene.radiance[i]);
		
		for (int i = 2; i < 48; i+=3)
			floats.put(scene.radiance[i]);

		floats.rewind();
		
		glUseProgram(shaders);
		glUniformMatrix4(light, false, floats);
		
		glUniform1i(glGetUniformLocation(shaders, "normals"), 0);
		glActiveTexture(GL_TEXTURE0);
		normals.bind();
		
		glUniform1i(glGetUniformLocation(shaders, "colors"), 1);
		glActiveTexture(GL_TEXTURE1);
		color.bind();
	}
	
	public void draw() {
		int normal = glGetAttribLocation(shaders, "normal");
		int coords = glGetAttribLocation(shaders, "coords");
		int visibility = glGetAttribLocation(shaders, "visibility");
	
		glBegin(GL_TRIANGLES);
		
		for (Face face : model.faces)
			for (Vertex vertex : face) {
				glVertexAttrib2f(coords, vertex.coords.x, vertex.coords.y);
				glVertexAttrib3f(normal, vertex.normal.x, vertex.normal.y, vertex.normal.z);
				glVertexAttrib4f(visibility, vertex.visibility.m00, vertex.visibility.m01, vertex.visibility.m02, vertex.visibility.m03);
				glVertexAttrib4f(visibility+1, vertex.visibility.m10, vertex.visibility.m11, vertex.visibility.m12, vertex.visibility.m13);
				glVertexAttrib4f(visibility+2, vertex.visibility.m20, vertex.visibility.m21, vertex.visibility.m22, vertex.visibility.m23);
				glVertexAttrib4f(visibility+3, vertex.visibility.m30, vertex.visibility.m31, vertex.visibility.m32, vertex.visibility.m33);
				glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
			}
		
		glEnd();
	}

	Model model;
	Scene scene;
	Texture normals = TextureLoader.getTexture("tga", ResourceLoader.getResourceAsStream("../Models/hand/Normal.tga"));
	Texture color = TextureLoader.getTexture("jpg", ResourceLoader.getResourceAsStream("../Models/hand/Color.jpg"));
	
	int shaders;
}

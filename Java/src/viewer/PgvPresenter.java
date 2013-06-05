package viewer;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL20.*;
import org.lwjgl.opengl.Drawable;

import java.nio.FloatBuffer;

import formats.pgv.Face;
import formats.pgv.Model;
import formats.pgv.Vertex;
import graphics.Buffers;
import graphics.Shaders;

class PgvPresenter extends AnimatedPresenter {
	public PgvPresenter(Drawable drawable, String file) throws Exception {
		model = new Model(file);
		shaders = Shaders.link(
				Shaders.compile("shader/Flat.vert", GL_VERTEX_SHADER),
				Shaders.compile("shader/Flat.frag", GL_FRAGMENT_SHADER));
		
		startup(drawable);
	    FloatBuffer floats = Buffers.allocateFloats(48);
	    
		for (int i = 0; i < 48; i+=3)
			floats.put(scene.radiance[i]);
		
		for (int i = 1; i < 48; i+=3)
			floats.put(scene.radiance[i]);
		
		for (int i = 2; i < 48; i+=3)
			floats.put(scene.radiance[i]);

		floats.rewind();
		
		glUseProgram(shaders);
		glUniformMatrix4(glGetUniformLocation(shaders, "light"), false, floats);
	}
	
	public void render(int pose) {
		model.setPose(pose);
	}
	
	public void draw() {
		int normal = glGetAttribLocation(shaders, "normal");
		int visibility = glGetAttribLocation(shaders, "visibility");
	
		glBegin(GL_TRIANGLES);
		
		for (Face face : model.faces)
			for (Vertex vertex : face) {
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
	int shaders;
}

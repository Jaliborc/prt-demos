package viewer;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL20.*;
import org.lwjgl.opengl.Drawable;

import java.nio.FloatBuffer;

import formats.pgv.Face;
import formats.pgv.PlainModel;
import formats.pgv.Edge;
import graphics.Buffers;
import graphics.Shaders;

class PgvPresenter extends AnimatedPresenter {
	public PgvPresenter(Drawable drawable, String file) throws Exception {
		model = new PlainModel(file);
		numPoses = model.numPoses();
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
	
	public void draw() throws Exception {
		int normal = glGetAttribLocation(shaders, "normal");
		int visibility = glGetAttribLocation(shaders, "visibility");
		super.draw();
	
		glUseProgram(shaders);
		glBegin(GL_TRIANGLES);
		
		for (Face face : model.faces)
			for (Edge edge : face) {
				glVertexAttrib3f(normal, edge.vertex.normal.x, edge.vertex.normal.y, edge.vertex.normal.z);
				glVertexAttrib4f(visibility, edge.vertex.visibility.m00, edge.vertex.visibility.m01, edge.vertex.visibility.m02, edge.vertex.visibility.m03);
				glVertexAttrib4f(visibility+1, edge.vertex.visibility.m10, edge.vertex.visibility.m11, edge.vertex.visibility.m12, edge.vertex.visibility.m13);
				glVertexAttrib4f(visibility+2, edge.vertex.visibility.m20, edge.vertex.visibility.m21, edge.vertex.visibility.m22, edge.vertex.visibility.m23);
				glVertexAttrib4f(visibility+3, edge.vertex.visibility.m30, edge.vertex.visibility.m31, edge.vertex.visibility.m32, edge.vertex.visibility.m33);
				glVertex3f(edge.vertex.position.x, edge.vertex.position.y, edge.vertex.position.z);
			}
		
		glEnd();
		glUseProgram(0);
	}
	
	PlainModel model;
	int shaders;
}

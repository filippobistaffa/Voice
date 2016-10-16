import nl.uu.cs.treewidth.input.GraphInput.InputData;
import nl.uu.cs.treewidth.ngraph.*;
import java.util.stream.Stream;
import java.util.ArrayList;
import java.util.HashSet;

public class Order {

	static private void process(String line, NGraph<InputData> g, HashSet<Integer> vertices,
				    ArrayList<NVertex<InputData>> array) {

		int nspaces = line.length() - line.replace(" ", "").length();

		if (nspaces > 0) {

			NVertex<InputData> v1, v2;
			String[] parts = line.split(" ");

			int v1id = Integer.parseInt(parts[0]);
			if (vertices.contains(v1id)) v1 = array.get(v1id);
			else {
				vertices.add(v1id);
				v1 = new ListVertex<InputData>();
				v1.data = new InputData();
				v1.data.id = v1id;
				v1.data.name = Integer.toString(v1id);
				array.add(v1id, v1);
				g.addVertex(v1);
			}

			int v2id = Integer.parseInt(parts[1]);
			if (vertices.contains(v2id)) v2 = array.get(v2id);
			else {
				vertices.add(v2id);
				v2 = new ListVertex<InputData>();
				v2.data = new InputData();
				v2.data.id = v2id;
				v2.data.name = Integer.toString(v2id);
				array.add(v2id, v2);
				g.addVertex(v2);
			}

			g.addEdge(v1, v2);
		}
	}

	static public void main(String arg[]) throws java.io.IOException {

		NGraph<InputData> g = new ListGraph<InputData>();
		HashSet<Integer> vertices = new HashSet<Integer>();
		ArrayList<NVertex<InputData>> array = new ArrayList<NVertex<InputData>>();

		try (Stream<String> lines = java.nio.file.Files.lines(java.nio.file.Paths.get(arg[0]))) {
			lines.forEachOrdered(line -> process(line, g, vertices, array));
		}
	}
}

#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/mesh/io/gmsh.hpp>
#include <es_fe/mesh/io/gmsh_file_reader.hpp>

#include <memory>
#include <string>
#include <vector>

namespace es_fe
{
std::unique_ptr<Mesh2> read_gmsh_mesh(const std::string& file_name, double scale)
{
	internal::Gmsh_file_reader reader(file_name);
	auto mesh = reader.read_mesh(scale);
	assert(!mesh->check());
	return mesh;
}

std::vector<unsigned int> read_gmsh_tags(const std::string& file_name, unsigned int tag_index)
{
	internal::Gmsh_file_reader reader(file_name);
	return reader.read_tags(tag_index);
}
} // namespace es_fe

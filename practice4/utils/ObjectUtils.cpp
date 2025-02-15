#include "ObjectUtils.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <map>

namespace {

    template<typename ... Args>
    std::string to_string(Args const &... args) {
        std::ostringstream os;
        (os << ... << args);
        return os.str();
    }

}

void fillModelFromFile(Model *model, std::filesystem::path const &path) {
    std::ifstream is(path);

    std::vector<std::array<float, 3>> positions;
    std::vector<std::array<float, 3>> normals;
    std::vector<std::array<float, 2>> texcoords;

    std::map<std::array<std::uint32_t, 3>, std::uint32_t> index_map;

    std::string line;
    std::size_t line_count = 0;

    auto fail = [&](auto const &... args) {
        throw std::runtime_error(to_string("Error parsing OBJ data, line ", line_count, ": ", args...));
    };

    while (std::getline(is >> std::ws, line)) {
        ++line_count;

        if (line.empty()) continue;

        if (line[0] == '#') continue;

        std::istringstream ls(std::move(line));

        std::string tag;
        ls >> tag;

        if (tag == "v") {
            auto &p = positions.emplace_back();
            ls >> p[0] >> p[1] >> p[2];
        } else if (tag == "vn") {
            auto &n = normals.emplace_back();
            ls >> n[0] >> n[1] >> n[2];
        } else if (tag == "vt") {
            auto &t = texcoords.emplace_back();
            ls >> t[0] >> t[1];
        } else if (tag == "f") {
            std::vector<std::uint32_t> vertices;

            while (ls) {
                std::array<std::uint32_t, 3> index{0, 0, 0};

                ls >> index[0];
                if (ls.eof()) break;
                if (!ls)
                    fail("expected position index");

                if (!std::isspace(ls.peek()) && !ls.eof()) {
                    if (ls.get() != '/')
                        fail("expected '/'");

                    if (ls.peek() != '/') {
                        ls >> index[1];
                        if (!ls)
                            fail("expected texcoord index");

                        if (!std::isspace(ls.peek()) && !ls.eof()) {
                            if (ls.get() != '/')
                                fail("expected '/'");

                            ls >> index[2];
                            if (!ls)
                                fail("expected normal index");
                        }
                    } else {
                        ls.get();

                        ls >> index[2];
                        if (!ls)
                            fail("expected normal index");
                    }
                }

                --index[0];
                --index[1];
                --index[2];

                if (index[0] >= positions.size())
                    fail("bad position index (", index[0], ")");

                if (index[1] != -1 && index[1] >= texcoords.size())
                    fail("bad texcoord index (", index[1], ")");

                if (index[2] != -1 && index[2] >= normals.size())
                    fail("bad normal index (", index[2], ")");

                auto it = index_map.find(index);
                if (it == index_map.end()) {
                    it = index_map.insert({index, model->getVertices()->size()}).first;

                    auto &v = model->getVertices()->emplace_back();

                    v.position = positions[index[0]];

                    if (index[1] != -1)
                        v.texcoord = texcoords[index[1]];
                    else
                        v.texcoord = {0.f, 0.f};

                    if (index[2] != -1)
                        v.normal = normals[index[2]];
                    else
                        v.normal = {0.f, 0.f, 0.f};
                }

                vertices.push_back(it->second);
            }

            for (std::size_t i = 1; i + 1 < vertices.size(); ++i) {
                model->getIndices()->push_back(vertices[0]);
                model->getIndices()->push_back(vertices[i]);
                model->getIndices()->push_back(vertices[i + 1]);
            }
        }
    }

}


Model parseModel(std::filesystem::path const &path) {
    Model result;
    fillModelFromFile(&result, path);
    return result;
}
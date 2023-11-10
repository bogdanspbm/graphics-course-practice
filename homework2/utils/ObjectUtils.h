#pragma once

#ifndef HOMEWORK2_OBJECTUTILS_H
#define HOMEWORK2_OBJECTUTILS_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "objects/graphics/renderable/Renderable.h"
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

inline void fillRenderableFromFile(Renderable *model, std::filesystem::path const &path) {
    std::ifstream is(path);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textCoords;

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
            ls >> p.x >> p.y >> p.z;
        } else if (tag == "vn") {
            auto &n = normals.emplace_back();
            ls >> n.x >> n.y >> n.z;
        } else if (tag == "vt") {
            auto &t = textCoords.emplace_back();
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

                if (index[1] != -1 && index[1] >= textCoords.size())
                    fail("bad texcoord index (", index[1], ")");

                if (index[2] != -1 && index[2] >= normals.size())
                    fail("bad normal index (", index[2], ")");

                auto it = index_map.find(index);
                if (it == index_map.end()) {
                    it = index_map.insert({index, model->getVertices()->size()}).first;

                    auto &v = model->getVertices()->emplace_back();

                    v.position = positions[index[0]];

                    if (index[1] != -1)
                        v.textCoord = textCoords[index[1]];
                    else
                        v.textCoord = {0.f, 0.f};

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

inline std::vector<Renderable *> loadRenderableListFromFile(std::filesystem::path const &path) {
    std::ifstream is(path);

    std::vector<Renderable *> output;

    Renderable *model = nullptr;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textCoords;

    std::map<std::array<std::uint32_t, 3>, std::uint32_t> index_map;

    std::string line;
    std::size_t line_count = 0;

    auto fail = [&](auto const &... args) {
        throw std::runtime_error(to_string("Error parsing OBJ data, line ", line_count, ": ", args...));
    };

    while (std::getline(is >> std::ws, line)) {
        ++line_count;

        if (line.empty()) continue;

        std::istringstream ls(std::move(line));

        std::string tag;
        ls >> tag;

        if (tag == "#") {
            std::string objectKeyword;
            ls >> objectKeyword;

            if (objectKeyword == "object") {
                std::string objectName;
                ls >> objectName;

                if (model != nullptr && model->getIndices()->size() != 0) {
                    model->generateBuffers();
                    output.push_back(model);
                }

                model = Renderable::getRenderableByName(objectName);
                model->setPath(path);
            }

            continue;
        }

        if (tag == "usemtl") {
            std::string materialName;
            ls >> materialName;
            model->setMaterialName(materialName);
            continue;
        }

        if (tag == "v") {
            auto &p = positions.emplace_back();
            ls >> p.x >> p.y >> p.z;
        } else if (tag == "vn") {
            auto &n = normals.emplace_back();
            ls >> n.x >> n.y >> n.z;
        } else if (tag == "vt") {
            auto &t = textCoords.emplace_back();
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

                if (index[1] != -1 && index[1] >= textCoords.size())
                    fail("bad texcoord index (", index[1], ")");

                if (index[2] != -1 && index[2] >= normals.size())
                    fail("bad normal index (", index[2], ")");

                auto it = index_map.find(index);
                if (it == index_map.end()) {
                    it = index_map.insert({index, model->getVertices()->size()}).first;

                    auto &v = model->getVertices()->emplace_back();

                    v.position = positions[index[0]];

                    if (index[1] != -1)
                        v.textCoord = textCoords[index[1]];
                    else
                        v.textCoord = {0.f, 0.f};

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

    return output;
}

inline std::vector<Material *> loadMaterialListFromFile(std::filesystem::path const &path) {
    std::ifstream is(path);

    std::vector<Material *> output;

    Material *material = nullptr;


    std::string line;
    std::size_t line_count = 0;

    auto fail = [&](auto const &... args) {
        throw std::runtime_error(to_string("Error parsing OBJ data, line ", line_count, ": ", args...));
    };

    while (std::getline(is >> std::ws, line)) {
        ++line_count;

        if (line.empty()) continue;

        std::istringstream ls(std::move(line));

        std::string tag;
        ls >> tag;

        if (tag == "#") {
            continue;
        }

        if (tag == "newmtl") {
            std::string materialName;
            ls >> materialName;

            if (material != nullptr) {
                output.push_back(material);
            }

            material = Material::getMaterialByName(materialName);
            continue;
        }

        if (tag == "Ns") {
            float glossines;
            ls >> glossines;
            material->setGlossiness(glossines);
        } else if (tag == "d") {
            float dissolve;
            ls >> dissolve;
            material->setOpacity(dissolve);
        } else if (tag == "Tr") {
            float transparent;
            ls >> transparent;
            material->setOpacity(1-transparent);
        } else if (tag == "Pr") {
            float roughness;
            ls >> roughness;
            material->setRoughness(roughness);
        } else if (tag == "map_Ka") {
            std::string textureName;
            ls >> textureName;

            for (char& c : textureName) {
                if (c == '\\') {
                    c = std::filesystem::path::preferred_separator;
                }
            }

            std::filesystem::path directory = path.parent_path();
            std::filesystem::path texturePath = directory / textureName;

            Texture *texture = Texture::getTexture(texturePath, DEFAULT);
            material->addTexture(texture);
        } else if (tag == "map_Kd") {
            std::string textureName;
            ls >> textureName;

            for (char& c : textureName) {
                if (c == '\\') {
                    c = std::filesystem::path::preferred_separator;
                }
            }

            std::filesystem::path directory = path.parent_path();
            std::filesystem::path texturePath = directory / textureName;

            Texture *texture = Texture::getTexture(texturePath, DEFAULT);
            material->addTexture(texture);
        } else if (tag == "map_bump") {
            std::string textureName;
            ls >> textureName;

            for (char& c : textureName) {
                if (c == '\\') {
                    c = std::filesystem::path::preferred_separator;
                }
            }

            std::filesystem::path directory = path.parent_path();
            std::filesystem::path texturePath = directory / textureName;

            Texture *texture = Texture::getTexture(texturePath, BUMP_MAP);
            material->addTexture(texture);
        } else if (tag == "map_d") {
            std::string textureName;
            ls >> textureName;

            for (char& c : textureName) {
                if (c == '\\') {
                    c = std::filesystem::path::preferred_separator;
                }
            }

            std::filesystem::path directory = path.parent_path();
            std::filesystem::path texturePath = directory / textureName;

            Texture *texture = Texture::getTexture(texturePath, DISPLACEMENT_MAP);
            material->addTexture(texture);
        } else if (tag == "map_Ks") {
            std::string textureName;
            ls >> textureName;

            for (char& c : textureName) {
                if (c == '\\') {
                    c = std::filesystem::path::preferred_separator;
                }
            }

            std::filesystem::path directory = path.parent_path();
            std::filesystem::path texturePath = directory / textureName;

            Texture *texture = Texture::getTexture(texturePath, SPECULAR_MAP);
            material->addTexture(texture);
        }

    }

    return output;
}


#endif //HOMEWORK2_OBJECTUTILS_H

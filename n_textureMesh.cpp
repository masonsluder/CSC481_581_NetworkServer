#include "n_textureMesh.h"

#include "n_component.h"

#include <iostream>

namespace N_Components {

	/**
		* TextureMesh constructor. Constructs a textureMesh component
		* with the given textureFilePath as its default.
		* @param textureFilePath to set during construction
		*/
	N_TextureMesh::N_TextureMesh(std::string textureFilePath) {
		m_textureFilePath = textureFilePath;
		m_isVisible = true;
	}

	void N_TextureMesh::update() {
		// TODO: Current animation state when available and applicable (if we ever do this)
	}

	/**
		* Returns the textureMesh's file path.
		* @return textureFilePath
		*/
	std::string N_TextureMesh::getTextureFilePath() {
		return m_textureFilePath;
	}

	/**
		* Sets the textureFilePath to the given string.
		* @param textureFilePath
		*/
	void N_TextureMesh::setTextureFilePath(std::string textureFilePath) {
		m_textureFilePath = textureFilePath;
	}

	/**
		* Gets the textureMesh's isVisible value.
		* @return true if the textureMesh is visible
		*/
	bool N_TextureMesh::getIsVisible() {
		return m_isVisible;
	}

	/**
		* Sets the textureMesh's visibility to the given boolean value.
		* @param isVisible
		*/
	void N_TextureMesh::setIsVisible(bool isVisible) {
		m_isVisible = isVisible;
	}
}

#include "GUITest.h"

void GUI_Render(ImDrawData *drawData)
{
	ImGuiIO &IO = ImGui::GetIO();

	float width = (float)(IO.DisplaySize.x * IO.DisplayFramebufferScale.x);
	float height = (float)(IO.DisplaySize.y * IO.DisplayFramebufferScale.y);

	if (width == 0 || height == 0)
	{
		return;
	}

	drawData->ScaleClipRects(IO.DisplayFramebufferScale);

	int lastTextureHandle;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureHandle);

	int lastProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
	int lastArrayBuffer;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
	int lastElementArrayBuffer;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &lastElementArrayBuffer);
	int lastBlendSrc;
	glGetIntegerv(GL_BLEND_SRC, &lastBlendSrc);
	int lastBlendDst;
	glGetIntegerv(GL_BLEND_DST, &lastBlendDst);
	int lastBlendEquationRGB;
	glGetIntegerv(GL_BLEND_EQUATION_RGB, &lastBlendEquationRGB);
	int lastBlendEquationAlpha;
	glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &lastBlendEquationAlpha);
	int lastViewPort[4];
	glGetIntegerv(GL_VIEWPORT, lastViewPort);
	bool lastEnableBlend = glIsEnabled(GL_BLEND);
	bool lastEnableCullFace = glIsEnabled(GL_CULL_FACE);
	bool lastEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
	bool lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	glViewport(0, 0, width, height);

	glUseProgram(GUIData.MainShader.ProgramHandle);

	float projMat[4][4] = 
	{
		{ 2.0f / IO.DisplaySize.x, 0.0f					   , 0.0f , 0.0f },
		{ 0.0f					 , 2.0f / -IO.DisplaySize.y, 0.0f , 0.0f },
		{ 0.0f					 , 0.0f					   , -1.0f, 0.0f },
		{ -1.0f					 , 1.0f					   , 0.0f , 1.0f },
	};

	glUniformMatrix4fv(GetUniformLocation(&GUIData.MainShader, UNIFORMS::PROJECTION_MATRIX), 1, GL_FALSE, &projMat[0][0]);
	glUniform1i(GetUniformLocation(&GUIData.MainShader, UNIFORMS::TEXTURE0), 0);
	GUI_BindBuffers();

	for (int n = 0; n < drawData->CmdListsCount; n++)
	{
		ImDrawList *cmdList = drawData->CmdLists[n];
		ImDrawIdx *idxBufferOffset = 0;

		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmdList->VtxBuffer.size() * sizeof(ImDrawVert), (void *)&cmdList->VtxBuffer.front(), GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GUIData.IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmdList->IdxBuffer.size() * sizeof(ImDrawIdx), (void *)&cmdList->IdxBuffer.front(), GL_STREAM_DRAW);

		for (const ImDrawCmd *cmd = cmdList->CmdBuffer.begin(); cmd != cmdList->CmdBuffer.end(); cmd++)
		{
			if (cmd->UserCallback)
			{
				cmd->UserCallback(cmdList, cmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (uint32)(intptr_t)cmd->TextureId);
				glScissor((int)cmd->ClipRect.x, (int)(height - cmd->ClipRect.w), (int)(cmd->ClipRect.z - cmd->ClipRect.x), (int)(cmd->ClipRect.w - cmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)cmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idxBufferOffset);
			}

			idxBufferOffset += cmd->ElemCount;
		}
	}

	glUseProgram(lastProgram);
	glBindTexture(GL_TEXTURE_2D, lastTextureHandle);
	glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lastElementArrayBuffer);
	glBlendEquationSeparate(lastBlendEquationRGB, lastBlendEquationAlpha);
	glBlendFunc(lastBlendSrc, lastBlendDst);
	if (lastEnableBlend)
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	if (lastEnableCullFace)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	if (lastEnableDepthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	if (lastEnableScissorTest)
	{
		glEnable(GL_SCISSOR_TEST);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
	}

	glViewport(lastViewPort[0], lastViewPort[1], lastViewPort[2], lastViewPort[3]);
}

void GUI_BindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, GUIData.VBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

#define OFFSET(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (void *)OFFSET(ImDrawVert, pos));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (void *)OFFSET(ImDrawVert, uv));
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (void *)OFFSET(ImDrawVert, col));
#undef OFFSET
}

void GUI_Init(float width, float height, GUI_Input *input)
{
	ImGuiIO &IO = ImGui::GetIO();
	
	IO.DisplaySize.x = width;
	IO.DisplaySize.y = height;

	IO.IniFilename = "imgui.ini";

	{
		IO.KeyMap[ImGuiKey_UpArrow] = input->UP.Button;
		IO.KeyMap[ImGuiKey_DownArrow] = input->DOWN.Button;
		IO.KeyMap[ImGuiKey_RightArrow] = input->RIGHT.Button;
		IO.KeyMap[ImGuiKey_LeftArrow] = input->LEFT.Button;
		IO.KeyMap[ImGuiKey_Backspace] = input->BACKSPACE.Button;
	}

	{
		IO.RenderDrawListsFn = GUI_Render;
	}

	{
		GUIData.MainShader = CreateShader("resources\\shaders\\GUI vertex shader 120.vert", "resources\\shaders\\GUI fragment shader 120.frag");
		AddUniform(&GUIData.MainShader, UNIFORMS::PROJECTION_MATRIX, "projectionMatrix");
		AddUniform(&GUIData.MainShader, UNIFORMS::TEXTURE0, "myTexture");
	}

	{
		uint8 *pixels;
		int32 width, height;

		IO.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		int lastTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		glGenTextures(1, &GUIData.TextureHandle);
		glBindTexture(GL_TEXTURE_2D, GUIData.TextureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		IO.Fonts->TexID = (void *)(intptr_t)GUIData.TextureHandle;

		glBindTexture(GL_TEXTURE_2D, lastTexture);
	}

	{
		int lastVertexArray;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArray);

		glGenBuffers(1, &GUIData.VBO);
		glGenBuffers(1, &GUIData.IBO);

		GUI_BindBuffers();

		glBindBuffer(GL_ARRAY_BUFFER, lastVertexArray);
	}
}

void GUI_ProcessInput(GUI_Input *input)
{
	ImGuiIO &IO = ImGui::GetIO();

	{
		for (size_t i = 0; i < GUI_BUTTON_COUNT; i++)
		{
			if (i < GUI_BUTTON_COUNT - 3)
			{
				IO.KeysDown[input->Buttons[i].Button] = input->Buttons[i].KeyDown;
			}
			else
			{				
				IO.MouseDown[i - (GUI_BUTTON_COUNT - 3)] = input->Buttons[i].KeyDown;
			}
		}		
	}

	{
		IO.MousePos = ImVec2(input->MouseX, input->MouseY);
	}

	if (input->Key.KeyUp)
	{
		IO.AddInputCharacter(char(input->Char));
	}
}

void GUI_NewFrame()
{
	ImGuiIO &IO = ImGui::GetIO();

	IO.DeltaTime = 1.0f / 60.0f;

	ImGui::NewFrame();
}
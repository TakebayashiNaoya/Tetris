#pragma once
#include "stdafx.h"

/// <summary>
/// �e�L�X�g�̏���ێ�����\���̂ł��B
/// </summary>
struct ResultInfo
{
	std::string text;	// ������B�i�����p�̂݁j
	Vector3 position;	// �\���ʒu�B
	float scale;		// �傫���B
	Vector4 color;		// �F�B
};

/// <summary>
/// �t�H���g�����_�[�̐ݒ�B
/// </summary>
/// <param name="pos_x">�`�悷��x���W�B</param>
/// <param name="pos_y">�`�悷��y���W�B</param>
/// <param name="scale">�`�悷��T�C�Y�B</param>
/// <param name="color">�`�悷��F�B</param>
/// <param name="fontRender">�ݒ肷��t�H���g�����_�[�B</param>
/// <param name="format">�`�悷�镶����B</param>
/// <param name="...">�`�悷�镶����𖳌��ɒǉ��ł���B</param>
static void SetTextOption(FontRender* fontRender, ResultInfo info, const wchar_t* format, ...)
{
	fontRender->SetPosition(info.position);
	fontRender->SetScale(info.scale);
	fontRender->SetColor(info.color);
	va_list _ArgList;
	__crt_va_start(_ArgList, format);
	wchar_t wcsbuf[256];
	vswprintf_s(wcsbuf, 256, format, _ArgList);
	__crt_va_end(_ArgList);
	fontRender->SetText(wcsbuf);
}
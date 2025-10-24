#pragma once
#include<SKFW/Framework/Singleton.hpp>
#include<SFML/Graphics.hpp>

class Camera : public Singleton<Camera>
{
	friend class Singleton<Camera>;
	Camera() = default;
	virtual~Camera() = default;
public:
	/// <summary>
	/// �J�����̏�����
	/// �E�B���h�E�̐������Ă���Ăяo��! �����΁I
	/// </summary>
	/// <param name="a_windowSize"></param>
	void Initialize(const sf::Vector2u& a_windowSize);

	/// <summary>
	/// �J�����̒��S�Z�b�g
	/// </summary>
	/// <param name="a_centerPosition">�J�����̍��W</param>
	void SetCenter(const sf::Vector2f& a_centerPosition);

	/// <summary>
	/// �J�����̑��Έړ�
	/// </summary>
	/// <param name="a_offset">�ړ���</param>
	void Move(const sf::Vector2f& a_offset);

	/// <summary>
	/// �Y�[���ݒ� 1.0�œ��{
	/// </summary>
	/// <param name="a_zoom"></param>
	void SetZoom(float a_zoom);

	/// <summary>
	/// ���[���h�p�J�������E�B���h�E�ɓK�p
	/// </summary>
	/// <param name="a_window"></param>
	void ApplyWorldView(sf::RenderWindow& a_window);

	/// <summary>
	/// �X�N���[���̃J�������E�B���h�E�ɓK��
	/// </summary>
	/// <param name="a_window"></param>
	void ApplySceneView(sf::RenderWindow& a_window);

	/// <summary>
	/// �J�����̎擾
	/// </summary>
	/// <returns></returns>
	const sf::View& GetView()const;

private:
	/// <summary>
	/// ���[���h�̃J�����r���[
	/// </summary>
	sf::View m_worldView;

	/// <summary>
	/// UI�p�̉�ʂ̃J�����r���[
	/// </summary>
	sf::View m_sceneView;

};
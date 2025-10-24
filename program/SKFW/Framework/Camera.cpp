#include<SKFW/Framework/Camera.h>


/// <summary>
/// �J�����̏�����
/// </summary>
/// <param name="a_windowSize"></param>
void Camera::Initialize(const sf::Vector2u& a_windowSize)
{
	sf::FloatRect viewRect(
		{0.0f
		,0.0f},
		{static_cast<float>(a_windowSize.x)
		,static_cast<float>(a_windowSize.y)});

	m_sceneView = sf::View(viewRect);
	m_worldView = m_sceneView;
}

/// <summary>
/// �J�����̒��S�Z�b�g
/// </summary>
/// <param name="a_centerPosition">�J�����̍��W</param>
void Camera::SetCenter(const sf::Vector2f& a_centerPosition)
{
	m_worldView.setCenter(a_centerPosition);
}

/// <summary>
/// �J�����̑��Έړ�
/// </summary>
/// <param name="a_offset">�ړ���</param>
void Camera::Move(const sf::Vector2f& a_offset)
{
	m_worldView.move(a_offset);
}

/// <summary>
/// �Y�[���ݒ� 1.0�œ��{
/// </summary>
/// <param name="a_zoom"></param>
void Camera::SetZoom(float a_zoom)
{
	m_worldView.setSize(m_sceneView.getSize() * a_zoom);
}

/// <summary>
/// ���[���h�p�J�������E�B���h�E�ɓK�p
/// </summary>
/// <param name="a_window"></param>
void Camera::ApplyWorldView(sf::RenderWindow& a_window)
{
	a_window.setView(m_worldView);
}

/// <summary>
/// �X�N���[���̃J�������E�B���h�E�ɓK��
/// </summary>
/// <param name="a_window"></param>
void Camera::ApplySceneView(sf::RenderWindow& a_window)
{
	a_window.setView(m_sceneView);
}

/// <summary>
/// �J�����̎擾
/// </summary>
/// <returns></returns>
const sf::View& Camera::GetView() const
{
	return m_worldView;
}

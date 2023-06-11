#include "pch.h"
#include "ATestState.h"
#include "Render/ATestModel.h"

#include <Toshi2/T2GUI/T2GUI.h>

AGameState::UpdateResult ATestState::OnUpdate(float deltaTime)
{
    return UpdateResult_OK;
}

void ATestState::OnRemoval()
{
    
}

void ATestState::OnInsertion()
{
    ATestModel::CreateSingleton();

    auto pGUI = Toshi::T2GUI::GetSingleton();
    auto pGUIRenderer = pGUI->GetRenderer();
    auto pRootElement = pGUI->GetRootElement();

    m_Menu.Create(320, 370);
    m_Menu.SetAnchor(Toshi::T2GUIElement::Anchor::BottomRight);
    m_Menu.SetPivot(Toshi::T2GUIElement::Pivot::BottomRight);
    m_Menu.SetColour(0x00202020);
    m_Menu.SetTransform(10, -10);

    Toshi::T2GUIRectangle* rectangle;
    
    rectangle = new Toshi::T2GUIRectangle;
    rectangle->Create(300, 50);
    rectangle->SetColour(0x00303030);
    rectangle->SetAnchor(Toshi::T2GUIElement::Anchor::TopCenter);
    rectangle->SetPivot(Toshi::T2GUIElement::Pivot::TopCenter);
    rectangle->SetTransform(0, 10);
    m_Menu.AddChildHead(rectangle);

    rectangle = new Toshi::T2GUIRectangle;
    rectangle->Create(300, 50);
    rectangle->SetColour(0x00343434);
    rectangle->SetAnchor(Toshi::T2GUIElement::Anchor::TopCenter);
    rectangle->SetPivot(Toshi::T2GUIElement::Pivot::TopCenter);
    rectangle->SetTransform(0, 70);
    m_Menu.AddChildHead(rectangle);

    rectangle = new Toshi::T2GUIRectangle;
    rectangle->Create(300, 50);
    rectangle->SetColour(0x00383838);
    rectangle->SetAnchor(Toshi::T2GUIElement::Anchor::TopCenter);
    rectangle->SetPivot(Toshi::T2GUIElement::Pivot::TopCenter);
    rectangle->SetTransform(0, 130);
    m_Menu.AddChildHead(rectangle);

    rectangle = new Toshi::T2GUIRectangle;
    rectangle->Create(300, 50);
    rectangle->SetColour(0x003C3C3C);
    rectangle->SetAnchor(Toshi::T2GUIElement::Anchor::TopCenter);
    rectangle->SetPivot(Toshi::T2GUIElement::Pivot::TopCenter);
    rectangle->SetTransform(0, 190);
    m_Menu.AddChildHead(rectangle);

    rectangle = new Toshi::T2GUIRectangle;
    rectangle->Create(300, 50);
    rectangle->SetColour(0x00404040);
    rectangle->SetAnchor(Toshi::T2GUIElement::Anchor::TopCenter);
    rectangle->SetPivot(Toshi::T2GUIElement::Pivot::TopCenter);
    rectangle->SetTransform(0, 250);
    m_Menu.AddChildHead(rectangle);

    rectangle = new Toshi::T2GUIRectangle;
    rectangle->Create(300, 50);
    rectangle->SetColour(0x00444444);
    rectangle->SetAnchor(Toshi::T2GUIElement::Anchor::TopCenter);
    rectangle->SetPivot(Toshi::T2GUIElement::Pivot::TopCenter);
    rectangle->SetTransform(0, 310);
    m_Menu.AddChildHead(rectangle);

    pRootElement->AddChildHead(&m_Menu);
}

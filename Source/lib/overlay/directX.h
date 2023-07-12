#pragma once



namespace overlay {

    class Direct2DRenderer
    {
    private:
        struct ImageData
        {
            std::wstring path;
            float x;
            float y;
            float rotation;
            float scale;
            bool active;
        };

    public:

        Direct2DRenderer(HWND hWnd) : m_hWnd(hWnd), m_pFactory(nullptr), m_pRenderTarget(nullptr)
        {
            Initialize();
        }

        ~Direct2DRenderer()
        {
            Cleanup();
        }

        void AddImage(const std::wstring& imagePath, float x, float y)
        {
            m_images.push_back({ imagePath, x, y, 0.0f, 1.0f, true });
        }

        ImageData& GetImage(int index) {
            return m_images[index];
        }

        void RotateImage(size_t index, float angle)
        {
            if (index < m_images.size())
            {
                m_images[index].rotation = angle;
            }
        }

        void ScaleImage(size_t index, float scaleFactor)
        {
            if (index < m_images.size())
            {
                m_images[index].scale = scaleFactor;
            }
        }

        void Resize(int width, int height)
        {
            if (m_pRenderTarget)
            {
                std::cout << "resize " << width << " " << height << "\n";
                m_pRenderTarget->Resize(D2D1::SizeU(width, height));
            }
        }

        ID2D1HwndRenderTarget* const GetRenderTarget() {
            return m_pRenderTarget;
        }

        void Render()
        {
            if (m_pRenderTarget)
            {
                m_pRenderTarget->BeginDraw();
                m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(0.f, 0.f, 0.f, 0.f)));;
        
                for (const auto& image : m_images)
                {
                    ID2D1Bitmap* pBitmap = LoadBitmapFromFile(image.path);
                    if (pBitmap && image.active)
                    {
                        D2D1_SIZE_F size = pBitmap->GetSize();
                        D2D1_RECT_F destRect = D2D1::RectF(image.x, image.y, image.x + size.width * image.scale, image.y + size.height * image.scale);
                        D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Rotation(image.rotation, D2D1::Point2F(image.x + size.width / 2, image.y + size.height / 2)) *
                            D2D1::Matrix3x2F::Scale(image.scale, image.scale, D2D1::Point2F(image.x, image.y));
                        m_pRenderTarget->SetTransform(transform);
                        m_pRenderTarget->DrawBitmap(pBitmap, destRect, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
                        pBitmap->Release();
                    }
                }
                m_pRenderTarget->EndDraw();
            }
        }

    private:

        HWND m_hWnd;
        ID2D1Factory* m_pFactory;
        ID2D1HwndRenderTarget* m_pRenderTarget;
        std::vector<ImageData> m_images;

        void Initialize()
        {
            HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
            if (SUCCEEDED(hr))
            {
                RECT rc;
                GetClientRect(m_hWnd, &rc);

                hr = m_pFactory->CreateHwndRenderTarget(
                    D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
                    D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
                    &m_pRenderTarget
                );
            }
        }

        void Cleanup()
        {
            if (m_pRenderTarget)
            {
                m_pRenderTarget->Release();
                m_pRenderTarget = nullptr;
            }

            if (m_pFactory)
            {
                m_pFactory->Release();
                m_pFactory = nullptr;
            }
        }

        ID2D1Bitmap* LoadBitmapFromFile(const std::wstring& imagePath)
        {
            IWICImagingFactory* pWICFactory = nullptr;
            IWICBitmapDecoder* pDecoder = nullptr;
            IWICBitmapFrameDecode* pFrame = nullptr;
            IWICFormatConverter* pConverter = nullptr;
            ID2D1Bitmap* pBitmap = nullptr;

            CoInitializeEx(NULL, COINIT_MULTITHREADED);
            HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pWICFactory));

            if (FAILED(hr))
                return nullptr;

            hr = pWICFactory->CreateDecoderFromFilename(imagePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
            if (FAILED(hr))
            {
                pWICFactory->Release();
                return nullptr;
            }

            hr = pDecoder->GetFrame(0, &pFrame);
            if (FAILED(hr))
            {
                pDecoder->Release();
                pWICFactory->Release();
                return nullptr;
            }

            hr = pWICFactory->CreateFormatConverter(&pConverter);
            if (FAILED(hr))
            {
                pFrame->Release();
                pDecoder->Release();
                pWICFactory->Release();
                return nullptr;
            }

            hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
            if (FAILED(hr))
            {
                pConverter->Release();
                pFrame->Release();
                pDecoder->Release();
                pWICFactory->Release();
                return nullptr;
            }

            hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, &pBitmap);
            if (FAILED(hr))
            {
                pConverter->Release();
                pFrame->Release();
                pDecoder->Release();
                pWICFactory->Release();
                return nullptr;
            }

            pConverter->Release();
            pFrame->Release();
            pDecoder->Release();
            pWICFactory->Release();

            return pBitmap;
        }
    };

    inline void rescaler(HWND own, HWND sow, Direct2DRenderer* dr) {
        RECT prevRect{};
        for (;;) {
            RECT rc;
            GetWindowRect(sow, &rc);
            if (prevRect.right != rc.right || prevRect.left != rc.left || prevRect.bottom != rc.bottom || prevRect.top != rc.top) {
                MoveWindow(own, rc.left, rc.top, (rc.right - rc.left) - 8, (rc.bottom - rc.top) - 8, true);
            }
            prevRect = rc;
            if (global::eject)
                return;
            Sleep(1);
        }
    }
}
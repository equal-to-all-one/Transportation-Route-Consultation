from django.urls import path
from rest_framework import routers
from . import views
from rest_framework_simplejwt.views import TokenObtainPairView, TokenRefreshView

router = routers.DefaultRouter()
router.register('cities', views.CityViewSet)
router.register('distances', views.DistanceViewSet)
router.register('flights', views.FlightViewSet)

urlpatterns = [
    path('token/', TokenObtainPairView.as_view()),
    path('token/refresh/', TokenRefreshView.as_view()),
    # path('csrftoken/', views.get_csrfToken),
    # path('inquire/', views.inquire)
]

urlpatterns += router.urls
import django_filters
from .models import Distance,FlightInfo


class DistanceFilter(django_filters.FilterSet):
    asA=django_filters.CharFilter(field_name="cityA")
    asB=django_filters.CharFilter(field_name="cityB")
    class Meta:
        model = Distance
        fields = ['cityA','cityB']


class FlightFilter(django_filters.FilterSet):
    low_price = django_filters.NumberFilter(field_name="price", lookup_expr="gte")
    high_price = django_filters.NumberFilter(field_name="price", lookup_expr="lte")
    earliest_time = django_filters.DateTimeFilter(field_name="depart_time", lookup_expr="gte")
    latest_time = django_filters.DateTimeFilter(field_name="arrive_time", lookup_expr="lte")
    departure_cityF =django_filters.CharFilter(field_name="departure_city")
    destination_cityF =django_filters.CharFilter(field_name="destination_city")
    class Meta:
        model = FlightInfo
        fields = ["price","departure_city", "destination_city", "depart_time", "arrive_time"]